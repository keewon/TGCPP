#include "TGMeasure.h"
#include "TGBeat.h"
#include <algorithm>

TGMeasure::TGMeasure(TGMeasureHeader* aHeader){
    track = NULL;
    header = aHeader;
    clef = TGMeasure_DEFAULT_CLEF;
    keySignature = TGMeasure_DEFAULT_KEY_SIGNATURE;
}

TGMeasure::~TGMeasure()
{
#ifndef AUTOMATIC_DELETE
    for (TGBeatList::iterator i=beats.begin(); i!=beats.end(); ++i)
    {
        delete *i;
    }
#endif

    beats.clear();
}
    
void TGMeasure::addBeat(TGBeat *beat){
    beat->setMeasure(this);
    beats.push_back(beat);
}
    
void TGMeasure::moveBeat(int index, TGBeat* beat){
    removeBeat(beat);
    beats.insert(beats.begin()+index,beat);
}
    
void TGMeasure::removeBeat(TGBeat *beat){

    TGBeatList::iterator i = std::find(
            beats.begin(), 
            beats.end(), 
            beat);
    if (i != beats.end())
    {
        beats.erase(i);
#ifndef AUTOMATIC_DELETE
        delete *i;
#endif
    }
}
    
TGBeat *TGMeasure::getBeat(int index){
    if(index >= 0 && index < countBeats()){
        return beats.at(index);
    }
    return NULL;
}
    
int TGMeasure::countBeats(){
    return beats.size();
}
    
    
void TGMeasure::makeEqual(TGMeasure* measure){
    clef = measure->getClef();
    keySignature = measure->getKeySignature();
    beats.clear();
    for(int i = 0; i < measure->countBeats(); i ++){
        TGBeat *beat = measure->getBeat(i);
        addBeat(beat);
    }
}
    
TGMeasure* TGMeasure::clone(TGFactory *factory,TGMeasureHeader *header){
    TGMeasure *measure = factory->newMeasure(header);
    measure->setClef(getClef());
    measure->setKeySignature(getKeySignature());

    for (TGBeatList::iterator i=beats.begin(); i!=beats.end(); ++i)
    {
        TGBeat *beat = *i;
        measure->addBeat(beat->clone(factory));
    }
    return measure;
}

