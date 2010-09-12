#ifndef TG_VOICE_H
#define TG_VOICE_H

#include "TGFactory.h"
#include "TGNote.h"

class TGVoice 
{
public:
    const static int DIRECTION_NONE = 0;
    const static int DIRECTION_UP = 1;
    const static int DIRECTION_DOWN = 2;
    
protected:
    TGBeat *beat;
    TGDuration *duration;
    TGNoteList notes;
    int index;
    int direction;
    bool empty;
    
public:
    TGVoice() 
    {
        beat = NULL;
        duration = NULL;
        index=0; direction=0; empty=false;
    }
    TGVoice(TGFactory *factory, int aIndex) {
        duration = factory->newDuration();
        index = aIndex;
        empty = true;
        direction = DIRECTION_NONE;
    }
    virtual ~TGVoice();
    
    int getIndex() {
        return index;
    }
    
    void setIndex(int aIndex) {
        index = aIndex;
    }
    
    bool isEmpty() {
        return empty;
    }
    
    void setEmpty(bool aEmpty) {
        empty = aEmpty;
    }
    
    int getDirection() {
        return direction;
    }
    
    void setDirection(int aDirection) {
        direction = aDirection;
    }
    
    TGDuration *getDuration() {
        return duration;
    }
    
    void setDuration(TGDuration *aDuration) {
        duration = aDuration;
    }
    
    TGBeat *getBeat() {
        return beat;
    }

    void setBeat(TGBeat *aBeat) {
        beat = aBeat;
    }

    TGNoteList & getNotes() {
        return notes;
    }
    
    void addNote(TGNote* aNote);
    
    void moveNote(int index,TGNote *aNote);
    
    void removeNote(TGNote *aNote);
    
    TGNote *getNote(int index){
        if(index >= 0 && index < countNotes()){
            return notes[index];
        }
        return NULL;
    }
    
    int countNotes(){
        return notes.size();
    }
    
    bool isRestVoice(){
        return notes.empty();
    }
    
    TGVoice *clone(TGFactory *factory);
    
};

#endif
