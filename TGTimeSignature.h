#ifndef TG_TIME_SIGNATURE_H
#define TG_TIME_SIGNATURE_H

#include "TGFactory.h"

class TGTimeSignature 
{
protected:
    TGDuration *denominator;
    int numerator;
    
public:
    TGTimeSignature(TGFactory *factory){
        numerator = 4;
        denominator = factory->newDuration();
    }

    virtual ~TGTimeSignature();
    
    int getNumerator() {
        return numerator;
    }
    
    void setNumerator(int aNumerator) {
        numerator = aNumerator;
    }
    
    TGDuration *getDenominator() {
        return denominator;
    }
    
    void setDenominator(TGDuration *aDenominator) {
        denominator = aDenominator;
    }
    
    TGTimeSignature* clone(TGFactory *factory){
        TGTimeSignature *timeSignature = factory->newTimeSignature();
        copy(timeSignature);
        return timeSignature;
    }
    
    void copy(TGTimeSignature *timeSignature);
    bool isEqual(TGTimeSignature *ts);
};

#endif
