#ifndef TG_EFFECT_TRILL
#define TG_EFFECT_TRILL

#include "TGFactory.h"
#include "TGDuration.h"

class TGEffectTrill {
    
protected:
    int fret;
    TGDuration *duration;
    
public:
    TGEffectTrill(TGFactory *factory) {
        fret = 0;
        duration = factory->newDuration();
    }
    
    int getFret() {
        return fret;
    }
    
    void setFret(int aFret) {
        fret = aFret;
    }
    
    TGDuration *getDuration() {
        return duration;
    }
    
    void setDuration(TGDuration *aDuration) {
        duration = aDuration;
    }
    
    TGEffectTrill* clone(TGFactory *factory){
        TGEffectTrill *effect = factory->newEffectTrill();
        effect->setFret(getFret());
        effect->getDuration()->setValue(getDuration()->getValue());
        effect->getDuration()->setDotted(getDuration()->isDotted());
        effect->getDuration()->setDoubleDotted(getDuration()->isDoubleDotted());
        effect->getDuration()->getTupleto()->setEnters(getDuration()->getTupleto()->getEnters());
        effect->getDuration()->getTupleto()->setTimes(getDuration()->getTupleto()->getTimes());
        
        return effect;
    }
    
};

#endif
