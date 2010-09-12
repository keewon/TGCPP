#ifndef TG_EFFECT_HARMONIC_H
#define TG_EFFECT_HARMONIC_H

#include "TGFactory.h"

#define TGEffectHarmonic_KEY_NATURAL "N.H"
#define TGEffectHarmonic_KEY_ARTIFICIAL "A.H"
#define TGEffectHarmonic_KEY_TAPPED "T.H"
#define TGEffectHarmonic_KEY_PINCH "P.H"
#define TGEffectHarmonic_KEY_SEMI "S.H"

#define TGEffectHarmonic_TYPE_NATURAL  1
#define TGEffectHarmonic_TYPE_ARTIFICIAL  2
#define TGEffectHarmonic_TYPE_TAPPED  3
#define TGEffectHarmonic_TYPE_PINCH  4
#define TGEffectHarmonic_TYPE_SEMI  5
#define TGEffectHarmonic_MIN_ARTIFICIAL_OFFSET  -24
#define TGEffectHarmonic_MAX_ARTIFICIAL_OFFSET  24
#define TGEffectHarmonic_MAX_TAPPED_OFFSET  24

class TGEffectHarmonic {
public:
    
    
/*
    static const int NATURAL_FREQUENCIES[][] = {
        {12, 12}, //AH12 (+12 frets)
        {9 , 28}, //AH9 (+28 frets)
        {5 , 24}, //AH5 (+24 frets)
        {7 , 19}, //AH7 (+19 frets)
        {4 , 28}, //AH4 (+28 frets)
        {3 , 31}  //AH3 (+31 frets)
    };
*/
    
protected:
    int type;
    int data;
    
public:
    TGEffectHarmonic(){
        type = 0;
        data = 0;
    }
    
    int getData() {
        return data;
    }
    
    void setData(int aData) {
        data = aData;
    }
    
    int getType() {
        return type;
    }
    
    void setType(int aType) {
        type = aType;
    }
    
    bool isNatural(){
        return (type == TGEffectHarmonic_TYPE_NATURAL);
    }
    
    bool isArtificial(){
        return (type == TGEffectHarmonic_TYPE_ARTIFICIAL);
    }
    
    bool isTapped(){
        return (type == TGEffectHarmonic_TYPE_TAPPED);
    }
    
    bool isPinch(){
        return (type == TGEffectHarmonic_TYPE_PINCH);
    }
    
    bool isSemi(){
        return (type == TGEffectHarmonic_TYPE_SEMI);
    }
    
    TGEffectHarmonic *clone(TGFactory *factory){
        TGEffectHarmonic *effect = factory->newEffectHarmonic();
        effect->setType(getType());
        effect->setData(getData());
        return effect;
    }
    
};

#endif

