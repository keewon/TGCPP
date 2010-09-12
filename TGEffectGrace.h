#ifndef TG_EFFECT_GRACE_H
#define TG_EFFECT_GRACE_H

#include "TGFactory.h"
#include "TGDuration.h"
#include "TGVelocities.h"

#define TGEffectGrace_TRANSITION_NONE  0
#define TGEffectGrace_TRANSITION_SLIDE  1
#define TGEffectGrace_TRANSITION_BEND  2
#define TGEffectGrace_TRANSITION_HAMMER  3
    

class TGEffectGrace {
    
protected:
    int fret;
    int duration;
    int dynamic;
    int transition;
    bool onBeat;
    bool dead;
    
public:
    TGEffectGrace() {
        fret = 0;
        duration = 1;
        dynamic = TGVelocities_DEFAULT;
        transition = TGEffectGrace_TRANSITION_NONE;
        onBeat = false;
        dead = false;
    }
    
    bool isDead() {
        return dead;
    }
    
    void setDead(bool aDead) {
        dead = aDead;
    }
    
    int getDuration() {
        return duration;
    }
    
    void setDuration(int aDuration) {
        duration = aDuration;
    }
    
    int getDynamic() {
        return dynamic;
    }
    
    void setDynamic(int aDynamic) {
        dynamic = aDynamic;
    }
    
    int getFret() {
        return fret;
    }
    
    void setFret(int aFret) {
        fret = aFret;
    }
    
    bool isOnBeat() {
        return onBeat;
    }
    
    void setOnBeat(bool aOnBeat) {
        onBeat = aOnBeat;
    }
    
    int getTransition() {
        return transition;
    }
    
    void setTransition(int aTransition) {
        transition = aTransition;
    }
    
    int getDurationTime(){
        return (int)((TGDuration_QUARTER_TIME / 16.00 ) * getDuration());
    }
    
    TGEffectGrace *clone(TGFactory *factory){
        TGEffectGrace *effect = factory->newEffectGrace();
        effect->setFret(getFret());
        effect->setDuration(getDuration());
        effect->setDynamic(getDynamic());
        effect->setTransition(getTransition());
        effect->setOnBeat(isOnBeat());
        effect->setDead(isDead());
        return effect;
    }
    
};


#endif
