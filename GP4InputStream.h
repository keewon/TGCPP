#ifndef GP4_INPUT_STREAM_H
#define GP4_INPUT_STREAM_H

#include "GP3InputStream.h"

class GP4InputStream: public GP3InputStream {
protected:
    int tripletFeel;
    
public:
    GP4InputStream();
    
    virtual TGSong *readSong();
    
protected:
    
    virtual void readTracks(TGSong *song, int count, TGChannelList *channels,TGLyric *lyric, int lyricTrack);
    virtual TGLyric *readLyrics();
    virtual TGNote *readNote(TGString *string, TGTrack *track,TGNoteEffect *effect);
    virtual void readChord(int strings,TGBeat *beat);
    virtual void readTremoloBar(TGNoteEffect *effect);
    virtual void readTremoloPicking(TGNoteEffect *effect);
    virtual void readNoteEffects(TGNoteEffect *noteEffect);
    virtual void readBeatEffects(TGBeat *beat,TGNoteEffect *noteEffect);
    virtual void readMixChange(TGTempo *tempo);
    
};

#endif // GP4_INPUT_STREAM_H
