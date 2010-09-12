#include "TGVoice.h"
#include "TGDuration.h"

#include <algorithm>

TGVoice:: ~TGVoice()
{
#ifndef AUTOMATIC_DELETE
    delete duration;
    duration = NULL;
#endif
}

void TGVoice::addNote(TGNote* note){
    note->setVoice(this);
    notes.push_back(note);
    setEmpty(false);
}

void TGVoice::moveNote(int index,TGNote *aNote)
{
    TGNoteList::iterator i = std::find(notes.begin(), notes.end(), aNote);
    if (i != notes.end())
    {
        notes.erase(i);
    }
    notes.insert(notes.begin() + index, aNote);
}

void TGVoice::removeNote(TGNote *aNote){
    TGNoteList::iterator i = std::find(notes.begin(), notes.end(), aNote);
    if (i != notes.end())
    {
        notes.erase(i);
#ifndef AUTOMATIC_DELETE
        delete (*i);
#endif
    }
}

TGVoice* TGVoice::clone(TGFactory *factory){
    TGVoice *voice = factory->newVoice(getIndex());
    voice->setEmpty(isEmpty());
    voice->setDirection( getDirection() );
    getDuration()->copy(voice->getDuration());
    for(TGNoteList::iterator i=notes.begin(); i!=notes.end(); ++i) {
        TGNote *note = (*i);
        voice->addNote(note->clone(factory));
    }
    return voice;
}
