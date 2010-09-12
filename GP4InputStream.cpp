#include "GP4InputStream.h"
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

#define GP4_SUPPORTED_VERSIONS1 "FICHIER GUITAR PRO v4.00"
#define GP4_SUPPORTED_VERSIONS2 "FICHIER GUITAR PRO v4.06"
#define GP4_SUPPORTED_VERSIONS3 "FICHIER GUITAR PRO L4.06"

#define GP_BEND_SEMITONE 25.0f
#define GP_BEND_POSITION 60.0f

GP4InputStream::GP4InputStream()
{
    versions[0] = GP4_SUPPORTED_VERSIONS1;
    versions[1] = GP4_SUPPORTED_VERSIONS2;
    versions[2] = GP4_SUPPORTED_VERSIONS3;

    for (int i=3; i<MAX_SUPPORTED_VERSION; ++i)
    {
        versions[i] = NULL;
    }

    stream = NULL;
}

TGSong *GP4InputStream::readSong() {
    readVersion();
    if (!isSupportedVersion(version.c_str())) {
        close();
        return NULL;
    }
    TGSong *song = getFactory()->newSong();

    readInfo(song);

    tripletFeel = ((readBoolean())?TGMeasureHeader_TRIPLET_FEEL_EIGHTH:TGMeasureHeader_TRIPLET_FEEL_NONE);

    int lyricTrack = readInt();
    TGLyric *lyric = readLyrics();

    int tempoValue = readInt();

    readInt(); //key

    readByte(); //octave

    TGChannelList *channels = readChannels();

    int measures = readInt();
    int tracks = readInt();

    readMeasureHeaders(song, measures);
    readTracks(song, tracks, channels, lyric, lyricTrack);
    readMeasures(song, measures, tracks, tempoValue);

    close();

    return song;
}

void GP4InputStream::readTracks(TGSong *song, int count, TGChannelList *channels,TGLyric *lyric, int lyricTrack) {
    for (int number = 1; number <= count; number++) {
        song->addTrack(readTrack(number, channels,(number == lyricTrack)?lyric:getFactory()->newLyric()));
    }
}

TGLyric *GP4InputStream::readLyrics() {
    TGLyric *lyric = getFactory()->newLyric();
    lyric->setFrom(readInt());
    lyric->setLyrics(readStringInteger());
    for (int i = 0; i < 4; i++) {
        readInt();
        readStringInteger();
    }
    return lyric;
}

TGNote *GP4InputStream::readNote(TGString *string, TGTrack *track,TGNoteEffect *effect) {
    int flags = readUnsignedByte();
    TGNote *note = getFactory()->newNote();
    note->setString(string->getNumber());
    note->setEffect(effect);
    note->getEffect()->setAccentuatedNote(((flags & 0x40) != 0));
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
    if ((flags & 0x08) != 0) {
        readNoteEffects(note->getEffect());
    }
    return note;
}

void GP4InputStream::readChord(int strings,TGBeat *beat) {
    TGChord *chord = getFactory()->newChord(strings);
    if ((readUnsignedByte() & 0x01) == 0) {
        chord->setName( readStringByteSizeOfInteger() );
        chord->setFirstFret(readInt());
        if(chord->getFirstFret() != 0){
            for (int i = 0; i < 6; i++) {
                int fret = readInt();
                if(i < chord->countStrings()){
                    chord->addFretValue(i,fret);
                }
            }
        }
    }
    else{
        skip(16);
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
    }
    if(chord->countNotes() > 0){
        beat->setChord(chord);
    }
}

void GP4InputStream::readTremoloBar(TGNoteEffect *effect) {
    TGEffectTremoloBar *tremoloBar = getFactory()->newEffectTremoloBar();
    skip(5);
    int points = readInt();
    for (int i = 0; i < points; i++) {
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

void GP4InputStream::readTremoloPicking(TGNoteEffect *effect) {
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

void GP4InputStream::readNoteEffects(TGNoteEffect *noteEffect) {
    int flags1 = readUnsignedByte();
    int flags2 = readUnsignedByte();

    noteEffect->setHammer(((flags1 & 0x02) != 0));
    noteEffect->setVibrato(((flags2 & 0x40) != 0) || noteEffect->isVibrato());
    noteEffect->setPalmMute(((flags2 & 0x02) != 0));
    noteEffect->setStaccato(((flags2 & 0x01) != 0));
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
        TGEffectHarmonic *harmonic = getFactory()->newEffectHarmonic();
        int type = readByte();
        if(type == 1){
            harmonic->setType(TGEffectHarmonic_TYPE_NATURAL);
        }else if(type == 3){
            harmonic->setType(TGEffectHarmonic_TYPE_TAPPED);
        }else if(type == 4){
            harmonic->setType(TGEffectHarmonic_TYPE_PINCH);
        }else if(type == 5){
            harmonic->setType(TGEffectHarmonic_TYPE_SEMI);
        }else if(type == 15){
            harmonic->setType(TGEffectHarmonic_TYPE_ARTIFICIAL);
            harmonic->setData(2);
        }else if(type == 17){
            harmonic->setType(TGEffectHarmonic_TYPE_ARTIFICIAL);
            harmonic->setData(3);
        }else if(type == 22){
            harmonic->setType(TGEffectHarmonic_TYPE_ARTIFICIAL);
            harmonic->setData(0);
        }
        noteEffect->setHarmonic(harmonic);
    }
    if ((flags2 & 0x20) != 0) {
        signed char fret = readByte();
        signed char period = readByte();
        TGEffectTrill *trill = getFactory()->newEffectTrill();
        trill->setFret(fret);
        if(period == 1){
            trill->getDuration()->setValue(TGDuration_SIXTEENTH);
            noteEffect->setTrill(trill);
        }else if(period == 2){
            trill->getDuration()->setValue(TGDuration_THIRTY_SECOND);
            noteEffect->setTrill(trill);
        }else if(period == 3){
            trill->getDuration()->setValue(TGDuration_SIXTY_FOURTH);
            noteEffect->setTrill(trill);
        }
    }
}

void GP4InputStream::readBeatEffects(TGBeat *beat,TGNoteEffect *noteEffect) {
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
    if ((flags2 & 0x02) != 0) {
        readByte();
    }
}

void GP4InputStream::readMixChange(TGTempo *tempo) {
    readByte();
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
    readByte();
}

