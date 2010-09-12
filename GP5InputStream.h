#ifndef GP5_INPUT_STREAM_H
#define GP5_INPUT_STREAM_H

#include "TGFactory.h"
#include "GTPInputStream.h"
#include "TGChannel.h"
#include "TGSong.h"
#include "TGMeasure.h"
#include "TGMeasureHeader.h"
#include "TGTrack.h"
#include "TGDuration.h"
#include "TGLyric.h"

#include <cmath>


class GP5InputStream : public GTPInputStream {
public:
    GP5InputStream();
    TGSong *readSong();
    
protected:
    void readInfo(TGSong *song);
    void readPageSetup();
    void readMeasureHeaders(TGSong *song, int count);
    void readTracks(TGSong *song, int count, TGChannelList *channels, TGLyric *lyric, int lyricTrack);
    void readMeasures(TGSong *song, int measures, int tracks, int tempoValue);
    TGLyric *readLyrics();
    long readBeat(long start, TGMeasure *measure, TGTrack *track, TGTempo *tempo, int voiceIndex);
    TGChannelList* readChannels();
    void readText(TGBeat *beat);
    TGDuration *readDuration(int flags);
    int getTiedNoteValue(int string, TGTrack *track);
    TGColor readColor();
    TGMarker *readMarker(int measure);
    TGMeasureHeader *readMeasureHeader(int index,TGTimeSignature *timeSignature);
    void readMeasure(TGMeasure *measure, TGTrack *track, TGTempo *tempo);
    TGNote *readNote(TGString *string,TGTrack *track,TGNoteEffect *effect);
    TGTrack *readTrack(int number, TGChannelList *channels,TGLyric *lyrics);
    void readChannel(TGChannel *channel,TGChannelList *channels);
    void readChord(int strings,TGBeat *beat);
    void readBeatEffects(TGBeat *beat, TGNoteEffect *noteEffect);
    void readNoteEffects(TGNoteEffect *noteEffect);
    void readGrace(TGNoteEffect *effect);
    void readBend(TGNoteEffect *effect);
    void readTremoloBar(TGNoteEffect *effect);
    void readTrill(TGNoteEffect *effect);
    void readArtificialHarmonic(TGNoteEffect *effect);
    void readTremoloPicking(TGNoteEffect *effect);
    void readMixChange(TGTempo *tempo);
    int toStrokeValue( int value );
    short toChannelShort(signed char b);
    TGBeat *getBeat(TGMeasure *measure, long start);
};

#endif // GP5_INPUT_STREAM_H
