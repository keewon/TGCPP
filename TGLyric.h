#ifndef TG_LYRIC_H
#define TG_LYRIC_H

#include "TGFactory.h"

class TGLyric 
{
protected:
    const static char REGEX = ' ';
    int from;
    TGStdString lyrics;
    
public:
    TGLyric(){
        from = 1;
    }
    
    int getFrom() {
        return from;
    }
    
    void setFrom(int _from) {
        from = _from;
    }
    
    TGStdString& getLyrics() {
        return lyrics;
    }
    
    void setLyrics(TGStdString _lyrics) {
        lyrics = _lyrics;
    }
    
    bool isEmpty(){
        return lyrics.size() == 0;
    }
    
    void copy(TGLyric* _lyric){
        _lyric->setFrom(getFrom());
        _lyric->setLyrics(getLyrics());
    }
    
};

#endif
