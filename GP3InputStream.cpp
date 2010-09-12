#include "GP3InputStream.h"
#include "TGFactory.h"
#include "GTPInputStream.h"
#include "TGChannel.h"
#include "TGSong.h"
#include "TGMeasure.h"
#include "TGMeasureHeader.h"
#include "TGTrack.h"
#include "TGDuration.h"
#include "TGBeat.h"
#include "TGVoice.h"
#include "TGNote.h"
#include "TGString.h"
#include "TGText.h"
#include "TGChord.h"
#include "TGStroke.h"
#include "TGEffectGrace.h"
#include "TGEffectHarmonic.h"
#include "TGEffectTremoloBar.h"
#include "TGEffectBend.h"
#include "TGEffectTrill.h"

#include <vector>
#include <cmath>

#define GP3_SUPPORTED_VERSIONS  "FICHIER GUITAR PRO v3.00"
#define GP_BEND_SEMITONE 25.0f
#define GP_BEND_POSITION 60.0f


GP3InputStream::GP3InputStream() 
{
    versions[0] = GP3_SUPPORTED_VERSIONS;

    for (int i=1; i<MAX_SUPPORTED_VERSION; ++i)
    {
        versions[i] = NULL;
    }

    stream = NULL;
}

TGSong* GP3InputStream::readSong() {
    readVersion();
    if (!isSupportedVersion( version.c_str() )) {
        close();
        return NULL;
    }
    TGSong *song = getFactory()->newSong();

    readInfo(song);

    tripletFeel = ((readBoolean())?TGMeasureHeader_TRIPLET_FEEL_EIGHTH:TGMeasureHeader_TRIPLET_FEEL_NONE);

    // DEBUG("tripletFeel: %d\n", tripletFeel); // OK

    int tempoValue = readInt();
    DEBUG("tempo: %d\n", tempoValue); // OK

    readInt(); //key

    TGChannelList *channels = readChannels();

    int measures = readInt();
    int tracks = readInt();

    readMeasureHeaders(song, measures);
    readTracks(song, tracks, channels);

    readMeasures(song, measures, tracks, tempoValue);

    close();

    return song;
}

TGChannelList* GP3InputStream::readChannels() {
    TGChannelList* channels = getFactory()->newChannelList();
    for (int i = 0; i < 64; i++) {
        TGChannel *channel = getFactory()->newChannel();
        channel->setChannel((short)i);
        channel->setEffectChannel((short)i);
        channel->setInstrument((short)readInt());
        channel->setVolume(toChannelShort(readByte()));
        channel->setBalance(toChannelShort(readByte()));
        channel->setChorus(toChannelShort(readByte()));
        channel->setReverb(toChannelShort(readByte()));
        channel->setPhaser(toChannelShort(readByte()));
        channel->setTremolo(toChannelShort(readByte()));
        channels->push_back(channel);
        skip(2);
    }
    return channels;
}

void GP3InputStream::readInfo(TGSong *song) {
    song->setName(readStringByteSizeOfInteger());
    //DEBUG("song->name: %s\n", song->getName().c_str());
    readStringByteSizeOfInteger();
    song->setArtist(readStringByteSizeOfInteger());
    //DEBUG("song->artist: %s\n", song->getArtist().c_str());
    song->setAlbum(readStringByteSizeOfInteger());
    //DEBUG("song->album: %s\n", song->getAlbum().c_str());
    song->setAuthor(readStringByteSizeOfInteger());
    //DEBUG("song->author: %s\n", song->getAuthor().c_str());
    readStringByteSizeOfInteger();
    readStringByteSizeOfInteger();
    readStringByteSizeOfInteger();
    int notes = readInt();
    //DEBUG("notes: %d\n", notes);
    for (int i = 0; i < notes; i++) {
        readStringByteSizeOfInteger();
    }
}

void GP3InputStream::readMeasureHeaders(TGSong *song, int count) {
    TGTimeSignature *timeSignature = getFactory()->newTimeSignature();
    for (int i = 0; i < count; i++) {
        song->addMeasureHeader(readMeasureHeader((i + 1),song,timeSignature));
    }
}

void GP3InputStream::readTracks(TGSong *song, int count, TGChannelList* channels) {
    for (int number = 1; number <= count; number++) {
        song->addTrack(readTrack(number, channels, NULL));
    }
}

void GP3InputStream::readMeasures(TGSong *song, int measures, int tracks, int tempoValue) {
    TGTempo *tempo = getFactory()->newTempo();
    tempo->setValue(tempoValue);
    long start = TGDuration_QUARTER_TIME;
    for (int i = 0; i < measures; i++) {
        TGMeasureHeader *header = song->getMeasureHeader(i);
        header->setStart(start);
        for (int j = 0; j < tracks; j++) {
            TGTrack* track = song->getTrack(j);
            TGMeasure *measure = getFactory()->newMeasure(header);
            track->addMeasure(measure);
            readMeasure(measure, track, tempo);
        }
        tempo->copy(header->getTempo());
        start += header->getLength();
    }
}

TGDuration* GP3InputStream::readDuration(int flags) {
    TGDuration *duration = getFactory()->newDuration();
    duration->setValue( (int) (pow( 2 , (readByte() + 4) ) / 4 ) );
    duration->setDotted(((flags & 0x01) != 0));
    if ((flags & 0x20) != 0) {
        int tuplet = readInt();
        switch (tuplet) {
            case 3:
                duration->getTupleto()->setEnters(3);
                duration->getTupleto()->setTimes(2);
                break;
            case 5:
                duration->getTupleto()->setEnters(5);
                duration->getTupleto()->setTimes(4);
                break;
            case 6:
                duration->getTupleto()->setEnters(6);
                duration->getTupleto()->setTimes(4);
                break;
            case 7:
                duration->getTupleto()->setEnters(7);
                duration->getTupleto()->setTimes(4);
                break;
            case 9:
                duration->getTupleto()->setEnters(9);
                duration->getTupleto()->setTimes(8);
                break;
            case 10:
                duration->getTupleto()->setEnters(10);
                duration->getTupleto()->setTimes(8);
                break;
            case 11:
                duration->getTupleto()->setEnters(11);
                duration->getTupleto()->setTimes(8);
                break;
            case 12:
                duration->getTupleto()->setEnters(12);
                duration->getTupleto()->setTimes(8);
                break;
        }
    }
    return duration;
}

int GP3InputStream::getTiedNoteValue(int string, TGTrack *track) {
    int measureCount = track->countMeasures();
    if (measureCount > 0) {
        for (int m = measureCount - 1; m >= 0; m--) {
            TGMeasure *measure = track->getMeasure( m );
            for (int b = measure->countBeats() - 1; b >= 0; b--) {
                TGBeat *beat = measure->getBeat( b );
                TGVoice *voice = beat->getVoice(0);  
                for (int n = 0; n < voice->countNotes(); n ++) {
                    TGNote *note = voice->getNote( n );
                    if (note->getString() == string) {
                        return note->getValue();
                    }
                }
            }
        }
    }
    return -1;
}

TGColor GP3InputStream::readColor() {
    unsigned char r = readUnsignedByte();
    unsigned char g = readUnsignedByte();
    unsigned char b = readUnsignedByte();
    read();

    return SET_COLOR(r, g, b);
}

TGMarker *GP3InputStream::readMarker(int measure) {
    TGMarker *marker = getFactory()->newMarker();
    marker->setMeasure(measure);
    marker->setTitle(readStringByteSizeOfInteger());
    marker->setColor( readColor() );
    return marker;
}

TGMeasureHeader* GP3InputStream::readMeasureHeader(int number,TGSong *song,TGTimeSignature *timeSignature) {
    int flags = readUnsignedByte();
    TGMeasureHeader *header = getFactory()->newHeader();
    header->setNumber(number);
    header->setStart(0);
    header->getTempo()->setValue(120);
    header->setTripletFeel(this->tripletFeel);
    header->setRepeatOpen( ((flags & 0x04) != 0) );

    if ((flags & 0x01) != 0) {
        timeSignature->setNumerator(readByte());
    }
    if ((flags & 0x02) != 0) {
        timeSignature->getDenominator()->setValue(readByte());
    }

    //DEBUG("time signature: %d/%d\n", timeSignature->getNumerator(), timeSignature->getDenominator()->getValue());
    timeSignature->copy(header->getTimeSignature());
    if ((flags & 0x08) != 0) {
        header->setRepeatClose(readByte());
    }
    if ((flags & 0x10) != 0) {
        header->setRepeatAlternative( parseRepeatAlternative(song, number, readUnsignedByte()) );
    }
    if ((flags & 0x20) != 0) {
        header->setMarker(readMarker(number));
    }
    if ((flags & 0x40) != 0) {
        readByte();
        readByte();
    }
    return header;
}

void GP3InputStream::readMeasure(TGMeasure *measure, TGTrack *track, TGTempo *tempo) {
    long nextNoteStart = measure->getStart();
    int numberOfBeats = readInt();
    //DEBUG("nextNoteStart: %ld, numofBeats=%d\n", nextNoteStart, numberOfBeats);
    for (int i = 0; i < numberOfBeats; i++) {
        nextNoteStart += readBeat(nextNoteStart, measure, track, tempo);
        //DEBUG(" nextNoteStart: %ld\n", nextNoteStart);
    }
}

long GP3InputStream::readBeat(long start, TGMeasure *measure,TGTrack *track, TGTempo *tempo) {
    int flags = readUnsignedByte();
    //DEBUG("beat flags: %x\n", flags);
    if((flags & 0x40) != 0){
        readUnsignedByte();
    }

    TGBeat *beat = getFactory()->newBeat();
    TGVoice *voice = beat->getVoice(0);
    TGDuration *duration = readDuration(flags);
    TGNoteEffect *effect = getFactory()->newEffect();
    if ((flags & 0x02) != 0) {
        readChord(track->stringCount(),beat);
    }
    if ((flags & 0x04) != 0) {
        readText(beat);
    }
    if ((flags & 0x08) != 0) {
        readBeatEffects(beat,effect);
    }
    if ((flags & 0x10) != 0) {
        readMixChange(tempo);
    }
    int stringFlags = readUnsignedByte();
    for (int i = 6; i >= 0; i--) {
        if ((stringFlags & (1 << i)) != 0 && (6 - i) < track->stringCount()) {
            TGString *string = track->getString( (6 - i) + 1 )->clone(getFactory());
            TGNote *note = readNote(string, track,effect->clone(getFactory()));
            voice->addNote(note);
        }
    }
    beat->setStart(start);
    voice->setEmpty(false);
    duration->copy(voice->getDuration());
    measure->addBeat(beat);

    return duration->getTime();
}

void GP3InputStream::readText(TGBeat *beat) {
    TGText *text = getFactory()->newText();
    text->setValue(readStringByteSizeOfInteger());
    beat->setText(text);
}

TGNote *GP3InputStream::readNote(TGString *string,TGTrack *track,TGNoteEffect *effect) {
    int flags = readUnsignedByte();
    TGNote *note = getFactory()->newNote();
    note->setString(string->getNumber());
    note->setEffect(effect);
    note->getEffect()->setAccentuatedNote(((flags & 0x40) != 0)); // GP4
    note->getEffect()->setGhostNote(((flags & 0x04) != 0));
    if ((flags & 0x20) != 0) {
        int noteType = readUnsignedByte();
        note->setTiedNote( (noteType == 0x02) );
        note->getEffect()->setDeadNote((noteType == 0x03));
    }
    if ((flags & 0x01) != 0) {
        skip(2);
    }
    if ((flags & 0x10) != 0) {
        note->setVelocity( (TGVelocities_MIN_VELOCITY + (TGVelocities_VELOCITY_INCREMENT * readByte())) - TGVelocities_VELOCITY_INCREMENT );
    }
    if ((flags & 0x20) != 0) {
        int fret = readByte();
        int value = ( note->isTiedNote() ? getTiedNoteValue(string->getNumber(), track) : fret );
        note->setValue( value >= 0 && value < 100 ? value : 0 );
    }
    if ((flags & 0x80) != 0) {
        skip(2);
    }
    if ((flags & 0x08) != 0) {
        readNoteEffects(note->getEffect());
    }
    return note;
}

TGTrack *GP3InputStream::readTrack(int number, TGChannelList* channels, TGLyric *lyrics) {
    TGTrack *track = getFactory()->newTrack();
    track->setNumber(number);
    track->setLyrics(lyrics); // GP4
    readUnsignedByte();
    track->setName(readStringByte(40));
    int stringCount = readInt();
    for (int i = 0; i < 7; i++) {
        int tuning = readInt();
        if (stringCount > i) {
            TGString *string = getFactory()->newString();
            string->setNumber(i + 1);
            string->setValue(tuning);
            track->getStrings().push_back(string);
        }
    }
    readInt();
    readChannel(track->getChannel(),channels);
    readInt();
    track->setOffset(readInt());
    track->setColor(readColor());
    return track;
}

void GP3InputStream::readChannel(TGChannel *channel, TGChannelList* channels) {
    int index = (readInt() - 1);
    int effectChannel = (readInt() - 1);
    
    if(index >= 0 && index < (int)channels->size()){
        channels->at(index)->copy(channel);
        if (channel->getInstrument() < 0) {
            channel->setInstrument((short)0);
        }
        if(!channel->isPercussionChannel()){
            channel->setEffectChannel((short)effectChannel);
        }
    }
}

int GP3InputStream::parseRepeatAlternative(TGSong *song,int measure,int value){
    int repeatAlternative = 0;
    int existentAlternatives = 0;
    TGMeasureHeaderList::iterator it = song->getMeasureHeaderIterator();

    while(it != song->getMeasureHeaderIteratorEnd()){
        TGMeasureHeader *header = (*it);
        if(header->getNumber() == measure){
            break;
        }
        if(header->isRepeatOpen()){
            existentAlternatives = 0;
        }
        existentAlternatives |= header->getRepeatAlternative();

        ++it;
    }

    for(int i = 0; i < 8; i ++){
        if(value > i && (existentAlternatives & (1 << i)) == 0){
            repeatAlternative |= (1 << i);
        }
    }
    return repeatAlternative;
}

void GP3InputStream::readChord(int strings, TGBeat* beat) {
    TGChord *chord = getFactory()->newChord(strings);
    int header = readUnsignedByte();
    if ((header & 0x01) == 0) {
        chord->setName(readStringByteSizeOfInteger());
        chord->setFirstFret(readInt());
        if (chord->getFirstFret() != 0) {
            for (int i = 0; i < 6; i++) {
                int fret = readInt();
                if(i < chord->countStrings()){
                    chord->addFretValue(i,fret);
                }
            }
        }
    }
    else{
        skip(25);
        chord->setName(readStringByte(34));
        chord->setFirstFret(readInt());
        for (int i = 0; i < 6; i++) {
            int fret = readInt();
            if(i < chord->countStrings()){
                chord->addFretValue(i,fret);
            }
        }
        skip(36);
    }
    if(chord->countNotes() > 0){
        beat->setChord(chord);
    }
}

void GP3InputStream::readGrace(TGNoteEffect *effect) {
    int fret = readUnsignedByte();
    TGEffectGrace *grace = getFactory()->newEffectGrace();
    grace->setOnBeat(false);
    grace->setDead( (fret == 255) );
    grace->setFret( ((!grace->isDead())?fret:0) );
    grace->setDynamic( (TGVelocities_MIN_VELOCITY + (TGVelocities_VELOCITY_INCREMENT * readUnsignedByte())) - TGVelocities_VELOCITY_INCREMENT );
    int transition = readUnsignedByte();
    if(transition == 0){
        grace->setTransition(TGEffectGrace_TRANSITION_NONE);
    }
    else if(transition == 1){
        grace->setTransition(TGEffectGrace_TRANSITION_SLIDE);
    }
    else if(transition == 2){
        grace->setTransition(TGEffectGrace_TRANSITION_BEND);
    }
    else if(transition == 3){
        grace->setTransition(TGEffectGrace_TRANSITION_HAMMER);
    }
    grace->setDuration(readUnsignedByte());
    effect->setGrace(grace);
}

void GP3InputStream::readBend(TGNoteEffect *effect) {
    TGEffectBend *bend = getFactory()->newEffectBend();
    skip(5);
    int points = readInt();
    for (int i = 0; i < points; i++) {
        int bendPosition = readInt();
        int bendValue = readInt();
        readByte(); //vibrato

        int pointPosition = round(bendPosition * TGEffectBend_MAX_POSITION_LENGTH / GP_BEND_POSITION);
        int pointValue = round(bendValue * TGEffectBend_SEMITONE_LENGTH / GP_BEND_SEMITONE);
        bend->addPoint(pointPosition,pointValue);
    }
    if(!bend->getPoints()->empty()){
        effect->setBend(bend);
    }
}

void GP3InputStream::readTremoloBar(TGNoteEffect *noteEffect) {
    int value = readInt();
    TGEffectTremoloBar *effect = getFactory()->newEffectTremoloBar();
    effect->addPoint(0,0);
    effect->addPoint( round(TGEffectTremoloBar_MAX_POSITION_LENGTH / 2.0f) ,round( -(value / (GP_BEND_SEMITONE * 2.0f) ) ) );
    effect->addPoint(TGEffectTremoloBar_MAX_POSITION_LENGTH,0);
    noteEffect->setTremoloBar(effect);
}

void GP3InputStream::readNoteEffects(TGNoteEffect *effect) {
    int flags = readUnsignedByte();
    effect->setSlide( ((flags & 0x04) != 0) );
    effect->setHammer( ((flags & 0x02) != 0) );
    if ((flags & 0x01) != 0) {
        readBend(effect);
    }
    if ((flags & 0x10) != 0) {
        readGrace(effect);
    }
}

void GP3InputStream::readBeatEffects(TGBeat *beat,TGNoteEffect *effect) {
    int flags = readUnsignedByte();
    effect->setVibrato(((flags & 0x01) != 0) || ((flags & 0x02) != 0));
    effect->setFadeIn(((flags & 0x10) != 0));
    if ((flags & 0x20) != 0) {
        int type = readUnsignedByte();
        if (type == 0) {
            readTremoloBar(effect);
        } else {
            effect->setTapping(type == 1);
            effect->setSlapping(type == 2);
            effect->setPopping(type == 3);
            readInt();
        }
    }
    if ((flags & 0x40) != 0) {
        int strokeDown = readByte();
        int strokeUp = readByte();
        if( strokeDown > 0 ){
            beat->getStroke()->setDirection( TGStroke_STROKE_DOWN );
            beat->getStroke()->setValue( toStrokeValue(strokeDown) );
        }else if( strokeUp > 0 ){
            beat->getStroke()->setDirection( TGStroke_STROKE_UP );
            beat->getStroke()->setValue( toStrokeValue(strokeUp) );
        }
    }
    if ((flags & 0x04) != 0) {
        TGEffectHarmonic *harmonic = getFactory()->newEffectHarmonic();
        harmonic->setType(TGEffectHarmonic_TYPE_NATURAL);
        effect->setHarmonic(harmonic);
    }
    if ((flags & 0x08) != 0) {
        TGEffectHarmonic *harmonic = getFactory()->newEffectHarmonic();
        harmonic->setType(TGEffectHarmonic_TYPE_ARTIFICIAL);
        harmonic->setData(0);
        effect->setHarmonic(harmonic);
    }
}

void GP3InputStream::readMixChange(TGTempo *tempo) {
    readByte(); //instrument
    int volume = readByte();
    int pan = readByte();
    int chorus = readByte();
    int reverb = readByte();
    int phaser = readByte();
    int tremolo = readByte();
    int tempoValue = readInt();
    if(volume >= 0){
        readByte();
    }
    if(pan >= 0){
        readByte();
    }
    if(chorus >= 0){
        readByte();
    }
    if(reverb >= 0){
        readByte();
    }
    if(phaser >= 0){
        readByte();
    }
    if(tremolo >= 0){
        readByte();
    }
    if(tempoValue >= 0){
        tempo->setValue(tempoValue);
        readByte();
    }
}

int GP3InputStream::toStrokeValue( int value ){
    if( value == 1 || value == 2){
        return TGDuration_SIXTY_FOURTH;
    }
    if( value == 3){
        return TGDuration_THIRTY_SECOND;
    }
    if( value == 4){
        return TGDuration_SIXTEENTH;
    }
    if( value == 5){
        return TGDuration_EIGHTH;
    }
    if( value == 6){
        return TGDuration_QUARTER;
    }
    return TGDuration_SIXTY_FOURTH;
}

short GP3InputStream::toChannelShort(unsigned char b){
    short value = (short)(( b * 8 ) - 1);
    return (short)(value>0?value:0);
}
