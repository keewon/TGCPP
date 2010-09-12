#ifndef TG_NOTE_EFFECT_H
#define TG_NOTE_EFFECT_H

#include "TGFactory.h"

class TGNoteEffect 
{
protected:
    TGEffectBend *bend;
    TGEffectTremoloBar *tremoloBar;
    TGEffectHarmonic *harmonic;
    TGEffectGrace *grace;
    TGEffectTrill *trill;
    TGEffectTremoloPicking *tremoloPicking;
    bool vibrato;
    bool deadNote;
    bool slide;
    bool hammer;
    bool ghostNote;
    bool accentuatedNote;
    bool heavyAccentuatedNote;
    bool palmMute;
    bool staccato;
    bool tapping;
    bool slapping;
    bool popping;
    bool fadeIn;
    
public:
    TGNoteEffect();
    bool isDeadNote();
    void setDeadNote(bool aDeadNote);
    bool isVibrato();
    void setVibrato(bool aVibrato);
    TGEffectBend* getBend();
    void setBend(TGEffectBend *aBend);
    bool isBend();
    TGEffectTremoloBar *getTremoloBar();
    void setTremoloBar(TGEffectTremoloBar *aTremoloBar);
    bool isTremoloBar();
    TGEffectTrill* getTrill();
    void setTrill(TGEffectTrill *aTrill);
    bool isTrill();
    TGEffectTremoloPicking *getTremoloPicking();
    void setTremoloPicking(TGEffectTremoloPicking *aTremoloPicking);
    bool isTremoloPicking();
    bool isHammer();
    void setHammer(bool aHammer);
    bool isSlide();
    void setSlide(bool aSlide);
    bool isGhostNote();
    void setGhostNote(bool aGhostNote);
    bool isAccentuatedNote();
    void setAccentuatedNote(bool aAccentuatedNote);
    bool isHeavyAccentuatedNote();
    void setHeavyAccentuatedNote(bool aHeavyAccentuatedNote);
    void setHarmonic(TGEffectHarmonic *aHarmonic);
    TGEffectHarmonic *getHarmonic();
    bool isHarmonic();
    TGEffectGrace *getGrace();
    void setGrace(TGEffectGrace *aGrace);
    bool isGrace();
    bool isPalmMute();
    void setPalmMute(bool aPalmMute);
    bool isStaccato();
    void setStaccato(bool aStaccato);
    bool isPopping();
    void setPopping(bool aPopping);
    bool isSlapping();
    void setSlapping(bool aSlapping);
    bool isTapping();
    void setTapping(bool aTapping);
    bool isFadeIn();
    void setFadeIn(bool aFadeIn);
    bool hasAnyEffect();
    TGNoteEffect *clone(TGFactory *factory);
    
};

#endif
