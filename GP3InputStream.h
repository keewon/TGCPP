#ifndef GP3_INPUT_STREAM_H
#define GP3_INPUT_STREAM_H

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


class GP3InputStream: public GTPInputStream {
protected:
    
    int tripletFeel;
    
public:
    GP3InputStream();
    virtual ~GP3InputStream() {}
    
    virtual TGSong* readSong();

protected:
    virtual TGChannelList* readChannels();
    virtual void readInfo(TGSong *song);
    virtual void readMeasureHeaders(TGSong *song, int count);
    virtual void readTracks(TGSong *song, int count, TGChannelList* channels);
    virtual void readMeasures(TGSong *song, int measures, int tracks, int tempoValue);
    virtual TGDuration* readDuration(int flags);
    virtual int getTiedNoteValue(int string, TGTrack *track);
    virtual TGColor readColor();
    virtual TGMarker *readMarker(int measure);
    
    virtual TGMeasureHeader* readMeasureHeader(int number,TGSong *song,TGTimeSignature *timeSignature);
    virtual void readMeasure(TGMeasure *measure, TGTrack *track, TGTempo *tempo);
    virtual long readBeat(long start, TGMeasure *measure,TGTrack *track, TGTempo *tempo);
    virtual void readText(TGBeat *beat);
    virtual TGNote *readNote(TGString *string,TGTrack *track,TGNoteEffect *effect);
    virtual TGTrack *readTrack(int number, TGChannelList* channels, TGLyric* lyrics);
    virtual void readChannel(TGChannel *channel, TGChannelList* channels);
    virtual int parseRepeatAlternative(TGSong *song,int measure,int value);
    virtual void readChord(int strings, TGBeat* beat);
    virtual void readGrace(TGNoteEffect *effect);
    virtual void readBend(TGNoteEffect *effect);
    virtual void readTremoloBar(TGNoteEffect *noteEffect);
    virtual void readNoteEffects(TGNoteEffect *effect);
    virtual void readBeatEffects(TGBeat *beat,TGNoteEffect *effect);
    virtual void readMixChange(TGTempo *tempo);
    virtual int toStrokeValue( int value );
    virtual short toChannelShort(unsigned char b);
};

#endif // GP3_INPUT_STREAM_H
