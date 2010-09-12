#ifndef TG_TEMPO_H
#define TG_TEMPO_H

#include "TGFactory.h"

class TGTempo 
{
protected:
    const static int SECOND_IN_MILLIS = 1000;
    int value;
    
public:
    TGTempo(){
        value = 120;
    }
    
    int getValue() {
        return value;
    }
    
    void setValue(int aValue) {
        value = aValue;
    }
    
    long getInMillis(){
        float millis = (60.0f / getValue() * SECOND_IN_MILLIS);
        return (long)millis;
    }
    
    long getInUSQ(){
        float usq = ((60.0f / getValue() * SECOND_IN_MILLIS) * 1000.0f);
        return (long)usq;
    }
    
    static TGTempo* fromUSQ(TGFactory *factory,int usq){
        float value = ((60.0f * SECOND_IN_MILLIS) / (usq / 1000.0f));
        TGTempo *tempo = factory->newTempo();
        tempo->setValue((int)value);
        return tempo;
    }
    
    TGTempo* clone(TGFactory *factory){
        TGTempo *tempo = factory->newTempo();
        copy(tempo);
        return tempo;
    }
    
    void copy(TGTempo *tempo){
        tempo->setValue(getValue());
    }
    
};

#endif
