#ifndef TG_SCALE_H
#define TG_SCALE_H

#include "TGFactory.h"

class TGScale 
{
protected:
    bool notes[12];
    int key;
    
public:
    TGScale(){
        clear();
    }
    
    void setKey(int aKey){
        key = aKey;
    }
    
    int getKey(){
        return key;
    }
    
    void setNote(int note, bool on){
        notes[note] = on;
    }
    
    bool getNote(int aNote){
        return notes[((aNote + (12 - key)) % 12)];
    }
    
    void clear(){
        setKey(0);
        for(int i = 0; i < 12; i++){
            setNote(i,false);
        }
    }
    
};

#endif
