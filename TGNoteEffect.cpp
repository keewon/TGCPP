#include "TGNoteEffect.h"
#include "TGEffect.h"

TGNoteEffect::TGNoteEffect(){
        bend = NULL;
        tremoloBar = NULL;
        harmonic = NULL;
        grace = NULL;
        trill = NULL;
        tremoloPicking = NULL;
        vibrato = false;
        deadNote = false;
        slide = false;
        hammer = false;
        ghostNote = false;
        accentuatedNote = false;
        heavyAccentuatedNote = false;
        palmMute = false;
        staccato = false;
        tapping = false;
        slapping = false;
        popping = false;
        fadeIn = false;
    }
    
    bool TGNoteEffect::isDeadNote() {
        return deadNote;
    }
    
    void TGNoteEffect::setDeadNote(bool aDeadNote) {
        deadNote = aDeadNote;

        if(isDeadNote()){
            tremoloBar = NULL;
            bend = NULL;
            trill = NULL;
            slide = false;
            hammer = false;
        }
    }
    
    bool TGNoteEffect::isVibrato() {
        return vibrato;
    }
    void TGNoteEffect::setVibrato(bool aVibrato) {
        vibrato = aVibrato;

        if(isVibrato()){
            trill = NULL;
        }
    }
    
    TGEffectBend* TGNoteEffect::getBend() {
        return bend;
    }
    
    void TGNoteEffect::setBend(TGEffectBend *aBend) {
        bend = aBend;

        if(isBend()){
            tremoloBar = NULL;
            trill = NULL;
            deadNote = false;
            slide = false;
            hammer = false;
        }
    }
    
    bool TGNoteEffect::isBend() {
        return (bend != NULL && !bend->getPoints()->empty());
    }
    
    TGEffectTremoloBar *TGNoteEffect::getTremoloBar() {
        return tremoloBar;
    }
    
    void TGNoteEffect::setTremoloBar(TGEffectTremoloBar *aTremoloBar) {
        tremoloBar = aTremoloBar;

        if(isTremoloBar()){
            bend = NULL;
            trill = NULL;
            deadNote = false;
            slide = false;
            hammer = false;
        }
    }
    
    bool TGNoteEffect::isTremoloBar() {
        return (tremoloBar != NULL);
    }
    
    
    TGEffectTrill* TGNoteEffect::getTrill() {
        return trill;
    }
    
    void TGNoteEffect::setTrill(TGEffectTrill *aTrill) {
        trill = aTrill;

        if(isTrill()){
            bend = NULL;
            tremoloBar = NULL;
            tremoloPicking = NULL;
            slide = false;
            hammer = false;
            deadNote = false;
            vibrato = false;
        }
    }
    
    bool TGNoteEffect::isTrill() {
        return (trill != NULL);
    }
    
    TGEffectTremoloPicking *TGNoteEffect::getTremoloPicking() {
        return tremoloPicking;
    }
    
    void TGNoteEffect::setTremoloPicking(TGEffectTremoloPicking *aTremoloPicking) {
        tremoloPicking = aTremoloPicking;

        if(isTremoloPicking()){
            trill = NULL;
            bend = NULL;
            tremoloBar = NULL;
            slide = false;
            hammer = false;
            deadNote = false;
            vibrato = false;
        }
    }
    
    bool TGNoteEffect::isTremoloPicking() {
        return (tremoloPicking != NULL);
    }
    
    bool TGNoteEffect::isHammer() {
        return hammer;
    }
    
    void TGNoteEffect::setHammer(bool aHammer) {
        hammer = aHammer;

        if(isHammer()){
            trill = NULL;
            tremoloBar = NULL;
            bend = NULL;
            deadNote = false;
            slide = false;
        }
    }
    
    bool TGNoteEffect::isSlide() {
        return slide;
    }
    
    void TGNoteEffect::setSlide(bool aSlide) {
        slide = aSlide;

        if(isSlide()){
            trill = NULL;
            tremoloBar = NULL;
            bend = NULL;
            deadNote = false;
            hammer = false;
        }
    }
    
    bool TGNoteEffect::isGhostNote() {
        return ghostNote;
    }
    
    void TGNoteEffect::setGhostNote(bool aGhostNote) {
        ghostNote = aGhostNote;

        if(isGhostNote()){
            accentuatedNote = false;
            heavyAccentuatedNote = false;
        }
    }
    
    bool TGNoteEffect::isAccentuatedNote() {
        return accentuatedNote;
    }
    
    void TGNoteEffect::setAccentuatedNote(bool aAccentuatedNote) {
        accentuatedNote = aAccentuatedNote;

        if(isAccentuatedNote()){
            ghostNote = false;
            heavyAccentuatedNote = false;
        }
    }
    
    bool TGNoteEffect::isHeavyAccentuatedNote() {
        return heavyAccentuatedNote;
    }
    
    void TGNoteEffect::setHeavyAccentuatedNote(bool aHeavyAccentuatedNote) {
        heavyAccentuatedNote = aHeavyAccentuatedNote;

        if(isHeavyAccentuatedNote()){
            ghostNote = false;
            accentuatedNote = false;
        }
    }
    
    void TGNoteEffect::setHarmonic(TGEffectHarmonic *aHarmonic) {
        harmonic = aHarmonic;
    }
    
    TGEffectHarmonic *TGNoteEffect::getHarmonic() {
        return harmonic;
    }
    
    bool TGNoteEffect::isHarmonic() {
        return (harmonic != NULL);
    }
    
    TGEffectGrace *TGNoteEffect::getGrace() {
        return grace;
    }
    
    void TGNoteEffect::setGrace(TGEffectGrace *aGrace) {
        grace = aGrace;
    }
    
    bool TGNoteEffect::isGrace() {
        return (grace != NULL);
    }
    
    bool TGNoteEffect::isPalmMute() {
        return palmMute;
    }
    
    void TGNoteEffect::setPalmMute(bool aPalmMute) {
        palmMute = aPalmMute;

        if(isPalmMute()){
            staccato = false;
        }
    }
    
    bool TGNoteEffect::isStaccato() {
        return staccato;
    }
    
    void TGNoteEffect::setStaccato(bool aStaccato) {
        staccato = aStaccato;

        if(isStaccato()){
            palmMute = false;
        }
    }
    
    bool TGNoteEffect::isPopping() {
        return popping;
    }
    
    void TGNoteEffect::setPopping(bool aPopping) {
        popping = aPopping;

        if(isPopping()){
            tapping = false;
            slapping = false;
        }
    }
    
    bool TGNoteEffect::isSlapping() {
        return slapping;
    }
    
    void TGNoteEffect::setSlapping(bool aSlapping) {
        slapping = aSlapping;

        if(isSlapping()){
            tapping = false;
            popping = false;
        }
    }
    
    bool TGNoteEffect::isTapping() {
        return tapping;
    }
    
    void TGNoteEffect::setTapping(bool aTapping) {
        tapping = aTapping;

        if(isTapping()){
            slapping = false;
            popping = false;
        }
    }
    
    bool TGNoteEffect::isFadeIn() {
        return fadeIn;
    }
    
    void TGNoteEffect::setFadeIn(bool aFadeIn) {
        fadeIn = aFadeIn;
    }
    
    bool TGNoteEffect::hasAnyEffect(){
        return (isBend() ||
                isTremoloBar() ||
                isHarmonic() ||
                isGrace() ||
                isTrill() ||
                isTremoloPicking() ||
                isVibrato() ||
                isDeadNote() ||
                isSlide() ||
                isHammer() ||
                isGhostNote() ||
                isAccentuatedNote() ||
                isHeavyAccentuatedNote() ||
                isPalmMute() ||
                isStaccato() ||
                isTapping() ||
                isSlapping() ||
                isPopping() ||
                isFadeIn());
    }
    
    TGNoteEffect* TGNoteEffect::clone(TGFactory *factory){
        TGNoteEffect *effect = factory->newEffect();
        effect->setVibrato(isVibrato());
        effect->setDeadNote(isDeadNote());
        effect->setSlide(isSlide());
        effect->setHammer(isHammer());
        effect->setGhostNote(isGhostNote());
        effect->setAccentuatedNote(isAccentuatedNote());
        effect->setHeavyAccentuatedNote(isHeavyAccentuatedNote());
        effect->setPalmMute(isPalmMute());
        effect->setStaccato(isStaccato());
        effect->setTapping(isTapping());
        effect->setSlapping(isSlapping());
        effect->setPopping(isPopping());
        effect->setFadeIn(isFadeIn());
        effect->setBend(isBend()?(TGEffectBend*)bend->clone(factory):NULL);
        effect->setTremoloBar(isTremoloBar()?(TGEffectTremoloBar*)tremoloBar->clone(factory):NULL);
        effect->setHarmonic(isHarmonic()?(TGEffectHarmonic*)harmonic->clone(factory):NULL);
        effect->setGrace(isGrace()?(TGEffectGrace*)grace->clone(factory):NULL);
        effect->setTrill(isTrill()?(TGEffectTrill*)trill->clone(factory):NULL);
        effect->setTremoloPicking(isTremoloPicking()?(TGEffectTremoloPicking*)tremoloPicking->clone(factory):NULL);
        return effect;
    }
