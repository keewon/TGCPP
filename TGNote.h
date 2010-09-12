#ifndef TG_NOTE_H
#define TG_NOTE_H

#include "TGFactory.h"
#include "TGVelocities.h"
#include "TGNoteEffect.h"

class TGNote 
{
protected:
    int value;
    int velocity;
    int string;
    bool tiedNote;
    TGNoteEffect *effect;
    TGVoice *voice;
    
public:
    TGNote(TGFactory *factory) {
        value = 0;
        velocity = TGVelocities_DEFAULT;
        string = 1;
        tiedNote = false;
        effect = factory->newEffect();
    }
    
    int getValue() {
        return value;
    }
    
    void setValue(int aValue) {
        value = aValue;
    }
    
    int getVelocity() {
        return velocity;
    }
    
    void setVelocity(int aVelocity) {
        velocity = aVelocity;
    }
    
    int getString() {
        return string;
    }
    
    void setString(int aString) {
        string = aString;
    }
    
    bool isTiedNote() {
        return tiedNote;
    }
    
    void setTiedNote(bool aTiedNote) {
        tiedNote = aTiedNote;
    }
    
    TGNoteEffect* getEffect() {
        return effect;
    }
    
    void setEffect(TGNoteEffect* aEffect) {
        effect = aEffect;
    }
    
    TGVoice* getVoice() {
        return voice;
    }
    
    void setVoice(TGVoice *aVoice) {
        voice = aVoice;
    }
    
    TGNote* clone(TGFactory *factory){
        TGNote* note = factory->newNote();
        note->setValue(getValue());
        note->setVelocity(getVelocity());
        note->setString(getString());
        note->setTiedNote(isTiedNote());
        note->setEffect(getEffect()->clone(factory));
        return note;
    }
};

#endif
