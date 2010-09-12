#ifndef TG_EFFECT_TREMOLO_PICKING
#define TG_EFFECT_TREMOLO_PICKING

#include "TGFactory.h"
#include "TGDuration.h"

class TGEffectTremoloPicking {
protected:   
    TGDuration *duration;
    
public:
    TGEffectTremoloPicking(TGFactory *factory) {
        duration = factory->newDuration();
    }
    
    TGDuration *getDuration() {
        return duration;
    }
    
    void setDuration(TGDuration *aDuration) {
        duration = aDuration;
    }
    
    TGEffectTremoloPicking *clone(TGFactory *factory){
        TGEffectTremoloPicking *effect = factory->newEffectTremoloPicking();
        effect->getDuration()->setValue(getDuration()->getValue());
        effect->getDuration()->setDotted(getDuration()->isDotted());
        effect->getDuration()->setDoubleDotted(getDuration()->isDoubleDotted());
        effect->getDuration()->getTupleto()->setEnters(getDuration()->getTupleto()->getEnters());
        effect->getDuration()->getTupleto()->setTimes(getDuration()->getTupleto()->getTimes());
        return effect;
    }
    
};


#endif
