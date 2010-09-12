#include "GP5InputStream.h"
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
#include "TGEffectTremoloPicking.h"

#include <cmath>

#define GP5_SUPPORTED_VERSIONS1 "FICHIER GUITAR PRO v5.00"
#define GP5_SUPPORTED_VERSIONS2 "FICHIER GUITAR PRO v5.10"

#define GP_BEND_SEMITONE 25.0f
#define GP_BEND_POSITION 60.0f

GP5InputStream::GP5InputStream()
{
    versions[0] = GP5_SUPPORTED_VERSIONS1;
    versions[1] = GP5_SUPPORTED_VERSIONS2;

    for (int i=2; i<MAX_SUPPORTED_VERSION; ++i)
    {
        versions[i] = NULL;
    }

    stream = NULL;
}

TGSong *GP5InputStream::readSong() {
    readVersion();
    if (!isSupportedVersion(version.c_str())) {
        close();
        return NULL;
    }
    TGSong *song = getFactory()->newSong();

    readInfo(song);

    //lyrics
    int lyricTrack = readInt();
    TGLyric *lyric = readLyrics();

    readPageSetup();

    int tempoValue = readInt();

    if(versionIndex > 0){
        skip(1);
    }

    readInt(); //key

    readByte(); //octave

    TGChannelList *channels = readChannels();

    skip(42);

    int measures = readInt();
    int tracks = readInt();

    readMeasureHeaders(song, measures);
    readTracks(song, tracks, channels, lyric, lyricTrack);

    readMeasures(song, measures, tracks, tempoValue);

    close();

    return song;
}

void GP5InputStream::readInfo(TGSong *song) {
    song->setName(readStringByteSizeOfInteger());
    readStringByteSizeOfInteger();
    song->setArtist(readStringByteSizeOfInteger());
    song->setAlbum(readStringByteSizeOfInteger());
    song->setAuthor(readStringByteSizeOfInteger());
    readStringByteSizeOfInteger();
    readStringByteSizeOfInteger();
    readStringByteSizeOfInteger();
    readStringByteSizeOfInteger();
    int notes = readInt();
    for (int i = 0; i < notes; i++) {
        readStringByteSizeOfInteger();
    }
}

void GP5InputStream::readPageSetup() {
    skip( (versionIndex > 0 ?49 : 30 ) );
    for (int i = 0; i < 11; i++) {
        skip(4);
        readStringByte(0);
    }
}

void GP5InputStream::readMeasureHeaders(TGSong *song, int count) {
    TGTimeSignature *timeSignature = getFactory()->newTimeSignature();
    for (int i = 0; i < count; i++) {
        if(i > 0 ){
            skip(1);
        }
        song->addMeasureHeader(readMeasureHeader(i,timeSignature));
    }
}

void GP5InputStream::readTracks(TGSong *song, int count, TGChannelList *channels,TGLyric *lyric, int lyricTrack) {
    for (int number = 1; number <= count; number++) {
        song->addTrack(readTrack(number, channels,(number == lyricTrack)?lyric:getFactory()->newLyric()));
    }
    skip( (versionIndex == 0 ? 2 : 1) );
}

void GP5InputStream::readMeasures(TGSong *song, int measures, int tracks, int tempoValue) {
    TGTempo *tempo = getFactory()->newTempo();
    tempo->setValue(tempoValue);
    long start = TGDuration_QUARTER_TIME;
    for (int i = 0; i < measures; i++) {
        TGMeasureHeader *header = song->getMeasureHeader(i);
        header->setStart(start);
        for (int j = 0; j < tracks; j++) {
            TGTrack *track = song->getTrack(j);
            TGMeasure *measure = getFactory()->newMeasure(header);
            track->addMeasure(measure);
            readMeasure(measure, track, tempo);
            skip(1);
        }
        tempo->copy(header->getTempo());
        start += header->getLength();
    }
}

TGLyric *GP5InputStream::readLyrics() {
    TGLyric *lyric = getFactory()->newLyric();
    lyric->setFrom(readInt());
    lyric->setLyrics(readStringInteger());
    for (int i = 0; i < 4; i++) {
        readInt();
        readStringInteger();
    }
    return lyric;
}

long GP5InputStream::readBeat(long start, TGMeasure *measure, TGTrack *track, TGTempo *tempo, int voiceIndex) {
    int flags = readUnsignedByte();

    TGBeat *beat = getBeat(measure, start);
    TGVoice *voice = beat->getVoice(voiceIndex);
    if((flags & 0x40) != 0){
        int beatType = readUnsignedByte();
        voice->setEmpty( ( beatType & 0x02 ) == 0 );
    }
    TGDuration *duration = readDuration(flags);
    TGNoteEffect *effect = getFactory()->newEffect();
    if ((flags & 0x02) != 0) {
        readChord(track->stringCount(), beat);
    }
    if ((flags & 0x04) != 0) {
        readText(beat);
    }
    if ((flags & 0x08) != 0) {
        readBeatEffects(beat, effect);
    }
    if ((flags & 0x10) != 0) {
        readMixChange(tempo);
    }
    int stringFlags = readUnsignedByte();
    for (int i = 6; i >= 0; i--) {
        if ((stringFlags & (1 << i)) != 0 && (6 - i) < track->stringCount()) {
            TGString *string = track->getString( (6 - i) + 1 )->clone(getFactory());
            TGNote *note = readNote(string,track,effect->clone(getFactory()));
            voice->addNote(note);
        }
        duration->copy(voice->getDuration());
    }

    skip(1);

    int read = readByte();
    if (read == 8 || read == 10) {
        skip(1);
    }

    return (!voice->isEmpty() ? duration->getTime() : 0 );
}

TGChannelList* GP5InputStream::readChannels() {
    TGChannelList *channels = getFactory()->newChannelList();
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

void GP5InputStream::readText(TGBeat *beat) {
    TGText *text = getFactory()->newText();
    text->setValue(readStringByteSizeOfInteger());
    beat->setText(text);
}

TGDuration *GP5InputStream::readDuration(int flags) {
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

int GP5InputStream::getTiedNoteValue(int string, TGTrack *track) {
    int measureCount = track->countMeasures();
    if (measureCount > 0) {
        for (int m = measureCount - 1; m >= 0; m--) {
            TGMeasure *measure = track->getMeasure( m );
            for (int b = measure->countBeats() - 1; b >= 0; b--) {
                TGBeat *beat = measure->getBeat( b );
                for (int v = 0; v < beat->countVoices(); v ++) {
                    TGVoice *voice = beat->getVoice(v);
                    if(!voice->isEmpty()){
                        for (int n = 0; n < voice->countNotes(); n ++) {
                            TGNote *note = voice->getNote( n );
                            if (note->getString() == string) {
                                return note->getValue();
                            }
                        }
                    }
                }
            }
        }
    }
    return -1;
}

TGColor GP5InputStream::readColor() {
    unsigned char r = readUnsignedByte();
    unsigned char g = readUnsignedByte();
    unsigned char b = readUnsignedByte();
    read();

    return SET_COLOR(r, g, b);
}

TGMarker *GP5InputStream::readMarker(int measure) {
    TGMarker *marker = getFactory()->newMarker();
    marker->setMeasure(measure);
    marker->setTitle(readStringByteSizeOfInteger());
    marker->setColor(readColor());
    return marker;
}

TGMeasureHeader *GP5InputStream::readMeasureHeader(int index,TGTimeSignature *timeSignature) {
    int flags = readUnsignedByte();
    TGMeasureHeader *header = getFactory()->newHeader();
    header->setNumber( (index + 1) );
    header->setStart(0);
    header->getTempo()->setValue(120);
    header->setRepeatOpen( ((flags & 0x04) != 0) );
    if ((flags & 0x01) != 0) {
        timeSignature->setNumerator(readByte());
    }
    if ((flags & 0x02) != 0) {
        timeSignature->getDenominator()->setValue(readByte());
    }
    timeSignature->copy(header->getTimeSignature());
    if ((flags & 0x08) != 0) {
        header->setRepeatClose( ( (readByte() & 0xff) - 1) );
    }
    if ((flags & 0x20) != 0) {
        header->setMarker(readMarker(header->getNumber()));
    }
    if ((flags & 0x10) != 0) {
        header->setRepeatAlternative(readUnsignedByte());
    }
    if ((flags & 0x40) != 0) {
        readByte();
        readByte();
    }
    if ((flags & 0x01) != 0) {
        skip(4);
    }
    if ((flags & 0x10) == 0) {
        skip(1);
    }
    int tripletFeel = readByte();
    if(tripletFeel == 1){
        header->setTripletFeel(TGMeasureHeader_TRIPLET_FEEL_EIGHTH);
    }else if(tripletFeel == 2){
        header->setTripletFeel(TGMeasureHeader_TRIPLET_FEEL_SIXTEENTH);
    }else{
        header->setTripletFeel(TGMeasureHeader_TRIPLET_FEEL_NONE);
    }
    return header;
}

void GP5InputStream::readMeasure(TGMeasure *measure, TGTrack *track, TGTempo *tempo) {
    for( int voice = 0 ; voice < 2 ; voice ++ ){
        long start = measure->getStart();
        int beats = readInt();
        for (int i = 0; i < beats; i++) {
            start += readBeat(start, measure, track, tempo, voice);
        }
    }

    TGBeatList emptyBeats;
    for( int i = 0 ; i < measure->countBeats() ; i ++ ){
        TGBeat *beat = measure->getBeat( i );
        bool empty = true;
        for( int v = 0 ; v < beat->countVoices() ; v ++ ){
            if( !beat->getVoice( v )->isEmpty() ){
                empty = false;
            }
        }
        if( empty ){
            emptyBeats.push_back( beat );
        }
    }

    for (TGBeatList::iterator it=emptyBeats.begin(); it!=emptyBeats.end(); ++it)
    {
        TGBeat *beat = (*it);
        measure->removeBeat( beat );
    }
}

TGNote *GP5InputStream::readNote(TGString *string,TGTrack *track,TGNoteEffect *effect) {
    int flags = readUnsignedByte();
    TGNote *note = getFactory()->newNote();
    note->setString(string->getNumber());
    note->setEffect(effect);
    note->getEffect()->setAccentuatedNote(((flags & 0x40) != 0));
    note->getEffect()->setHeavyAccentuatedNote(((flags & 0x02) != 0));
    note->getEffect()->setGhostNote(((flags & 0x04) != 0));
    if ((flags & 0x20) != 0) {
        int noteType = readUnsignedByte();
        note->setTiedNote( (noteType == 0x02) );
        note->getEffect()->setDeadNote((noteType == 0x03));
    }
    if ((flags & 0x10) != 0) {
        note->setVelocity((TGVelocities_MIN_VELOCITY + (TGVelocities_VELOCITY_INCREMENT * readByte())) - TGVelocities_VELOCITY_INCREMENT);
    }
    if ((flags & 0x20) != 0) {
        int fret = readByte();
        int value = ( note->isTiedNote() ? getTiedNoteValue(string->getNumber(), track) : fret );
        note->setValue( value >= 0 && value < 100 ? value : 0 );
    }
    if ((flags & 0x80) != 0) {
        skip(2);
    }
    if ((flags & 0x01) != 0) {
        skip(8);
    }
    skip(1);
    if ((flags & 0x08) != 0) {
        readNoteEffects(note->getEffect());
    }
    return note;
}

TGTrack *GP5InputStream::readTrack(int number, TGChannelList *channels,TGLyric *lyrics) {
    readUnsignedByte();
    if(number ==  1 || versionIndex == 0){
        skip(1);
    }
    TGTrack *track = getFactory()->newTrack();
    track->setNumber(number);
    track->setLyrics(lyrics);
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
    readChannel(track->getChannel(), channels);
    readInt();
    track->setOffset(readInt());
    track->setColor(readColor());
    skip( (versionIndex > 0)? 49 : 44);
    if(versionIndex > 0){
        readStringByteSizeOfInteger();
        readStringByteSizeOfInteger();
    }
    return track;
}

void GP5InputStream::readChannel(TGChannel *channel,TGChannelList *channels) {
    int index = (readInt() - 1);
    int effectChannel = (readInt() - 1);
    if(index >= 0 && index < (int)(channels->size())){
        channels->at(index)->copy(channel);
        if (channel->getInstrument() < 0) {
            channel->setInstrument((short)0);
        }
        if(!channel->isPercussionChannel()){
            channel->setEffectChannel((short)effectChannel);
        }
    }
}

void GP5InputStream::readChord(int strings,TGBeat *beat) {
    TGChord *chord = getFactory()->newChord(strings);
    skip(17);
    chord->setName(readStringByte(21));
    skip(4);
    chord->setFirstFret(readInt());
    for (int i = 0; i < 7; i++) {
        int fret = readInt();
        if(i < chord->countStrings()){
            chord->addFretValue(i,fret);
        }
    }
    skip(32);
    if(chord->countNotes() > 0){
        beat->setChord(chord);
    }
}

void GP5InputStream::readBeatEffects(TGBeat *beat, TGNoteEffect *noteEffect) {
    int flags1 = readUnsignedByte();
    int flags2 = readUnsignedByte();
    noteEffect->setFadeIn(((flags1 & 0x10) != 0));
    noteEffect->setVibrato(((flags1  & 0x02) != 0));
    if ((flags1 & 0x20) != 0) {
        int effect = readUnsignedByte();
        noteEffect->setTapping(effect == 1);
        noteEffect->setSlapping(effect == 2);
        noteEffect->setPopping(effect == 3);
    }
    if ((flags2 & 0x04) != 0) {
        readTremoloBar(noteEffect);
    }
    if ((flags1 & 0x40) != 0) {
        int strokeUp = readByte();
        int strokeDown = readByte();
        if( strokeUp > 0 ){
            beat->getStroke()->setDirection( TGStroke_STROKE_UP );
            beat->getStroke()->setValue( toStrokeValue(strokeUp) );
        }else if( strokeDown > 0 ){
            beat->getStroke()->setDirection( TGStroke_STROKE_DOWN );
            beat->getStroke()->setValue( toStrokeValue(strokeDown) );
        } 
    }
    if ((flags2 & 0x02) != 0) {
        readByte();
    }
}

void GP5InputStream::readNoteEffects(TGNoteEffect *noteEffect) {
    int flags1 = readUnsignedByte();
    int flags2 = readUnsignedByte();
    if ((flags1 & 0x01) != 0) {
        readBend(noteEffect);
    }
    if ((flags1 & 0x10) != 0) {
        readGrace(noteEffect);
    }
    if ((flags2 & 0x04) != 0) {
        readTremoloPicking(noteEffect);
    }
    if ((flags2 & 0x08) != 0) {
        noteEffect->setSlide(true);
        readByte();
    }
    if ((flags2 & 0x10) != 0) {
        readArtificialHarmonic(noteEffect);
    }
    if ((flags2 & 0x20) != 0) {
        readTrill(noteEffect);
    }
    noteEffect->setHammer(((flags1 & 0x02) != 0));
    noteEffect->setVibrato(((flags2 & 0x40) != 0) || noteEffect->isVibrato());
    noteEffect->setPalmMute(((flags2 & 0x02) != 0));
    noteEffect->setStaccato(((flags2 & 0x01) != 0));
}

void GP5InputStream::readGrace(TGNoteEffect *effect) {
    int fret = readUnsignedByte();
    int dynamic = readUnsignedByte();
    int transition = readByte();
    int duration = readUnsignedByte();
    int flags = readUnsignedByte();
    TGEffectGrace *grace = getFactory()->newEffectGrace();
    grace->setFret( fret );
    grace->setDynamic( (TGVelocities_MIN_VELOCITY + (TGVelocities_VELOCITY_INCREMENT * dynamic)) - TGVelocities_VELOCITY_INCREMENT );
    grace->setDuration(duration);
    grace->setDead( (flags & 0x01) != 0 );
    grace->setOnBeat( (flags & 0x02) != 0 );
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
    effect->setGrace(grace);
}

void GP5InputStream::readBend(TGNoteEffect *effect) {
    skip(5);
    TGEffectBend *bend = getFactory()->newEffectBend();
    int numPoints = readInt();
    for (int i = 0; i < numPoints; i++) {
        int bendPosition = readInt();
        int bendValue = readInt();
        readByte();

        int pointPosition = round(bendPosition * TGEffectBend_MAX_POSITION_LENGTH / GP_BEND_POSITION);
        int pointValue = round(bendValue * TGEffectBend_SEMITONE_LENGTH / GP_BEND_SEMITONE);
        bend->addPoint(pointPosition,pointValue);
    }
    if(!bend->getPoints()->empty()){
        effect->setBend(bend);
    }
}

void GP5InputStream::readTremoloBar(TGNoteEffect *effect) {
    skip(5);
    TGEffectTremoloBar *tremoloBar = getFactory()->newEffectTremoloBar();
    int numPoints = readInt();
    for (int i = 0; i < numPoints; i++) {
        int position = readInt();
        int value = readInt();
        readByte();

        int pointPosition = round(position * TGEffectTremoloBar_MAX_POSITION_LENGTH / GP_BEND_POSITION);
        int pointValue = round(value / (GP_BEND_SEMITONE * 2.0f));
        tremoloBar->addPoint(pointPosition,pointValue);
    }
    if(!tremoloBar->getPoints().empty()){
        effect->setTremoloBar(tremoloBar);
    }
}

void GP5InputStream::readTrill(TGNoteEffect *effect) {
    signed char fret = readByte();
    signed char period = readByte();
    TGEffectTrill *trill = getFactory()->newEffectTrill();
    trill->setFret(fret);
    if(period == 1){
        trill->getDuration()->setValue(TGDuration_SIXTEENTH);
        effect->setTrill(trill);
    }else if(period == 2){
        trill->getDuration()->setValue(TGDuration_THIRTY_SECOND);
        effect->setTrill(trill);
    }else if(period == 3){
        trill->getDuration()->setValue(TGDuration_SIXTY_FOURTH);
        effect->setTrill(trill);
    }
}

void GP5InputStream::readArtificialHarmonic(TGNoteEffect *effect) {
    int type = readByte();
    TGEffectHarmonic *harmonic = getFactory()->newEffectHarmonic();
    harmonic->setData(0);
    if(type == 1){
        harmonic->setType(TGEffectHarmonic_TYPE_NATURAL);
        effect->setHarmonic(harmonic);
    }else if(type == 2){
        skip(3);
        harmonic->setType(TGEffectHarmonic_TYPE_ARTIFICIAL);
        effect->setHarmonic(harmonic);
    }else if(type == 3){
        skip(1);
        harmonic->setType(TGEffectHarmonic_TYPE_TAPPED);
        effect->setHarmonic(harmonic);
    }else if(type == 4){
        harmonic->setType(TGEffectHarmonic_TYPE_PINCH);
        effect->setHarmonic(harmonic);
    }else if(type == 5){
        harmonic->setType(TGEffectHarmonic_TYPE_SEMI);
        effect->setHarmonic(harmonic);
    }
}

void GP5InputStream::readTremoloPicking(TGNoteEffect *effect) {
    int value = readUnsignedByte();
    TGEffectTremoloPicking *tp = getFactory()->newEffectTremoloPicking();
    if(value == 1){
        tp->getDuration()->setValue(TGDuration_EIGHTH);
        effect->setTremoloPicking(tp);
    }else if(value == 2){
        tp->getDuration()->setValue(TGDuration_SIXTEENTH);
        effect->setTremoloPicking(tp);
    }else if(value == 3){
        tp->getDuration()->setValue(TGDuration_THIRTY_SECOND);
        effect->setTremoloPicking(tp);
    }
}

void GP5InputStream::readMixChange(TGTempo *tempo) {
    readByte(); //instrument

    skip(16);
    int volume = readByte();
    int pan = readByte();
    int chorus = readByte();
    int reverb = readByte();
    int phaser = readByte();
    int tremolo = readByte();
    readStringByteSizeOfInteger(); //tempoName
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
        skip(1);
        if(versionIndex > 0){
            skip(1);
        }
    }
    readByte();
    skip(1);
    if(versionIndex > 0){
        readStringByteSizeOfInteger();
        readStringByteSizeOfInteger();
    }
}

int GP5InputStream::toStrokeValue( int value ){
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

short GP5InputStream::toChannelShort(signed char b){
    short value = (short)(( b * 8 ) - 1);
    return value > 0 ? value : 0;
}

TGBeat *GP5InputStream::getBeat(TGMeasure *measure, long start){
    int count = measure->countBeats();
    for(int i = 0 ; i < count ; i ++ ){
        TGBeat *beat = measure->getBeat( i );
        if( beat->getStart() == start ){
            return beat;
        }
    }
    TGBeat *beat = getFactory()->newBeat();
    beat->setStart(start);
    measure->addBeat(beat);
    return beat;
}
