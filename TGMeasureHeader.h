#ifndef TG_MEASURE_HEADER_H
#define TG_MEASURE_HEADER_H

#include "TGFactory.h"
#include "TGTimeSignature.h"
#include "TGTempo.h"
#include "TGMarker.h"
#include "TGSong.h"
#include "TGDuration.h"

#define TGMeasureHeader_TRIPLET_FEEL_NONE 1
#define TGMeasureHeader_TRIPLET_FEEL_EIGHTH 2
#define TGMeasureHeader_TRIPLET_FEEL_SIXTEENTH 3

class TGMeasureHeader 
{
public:
    
protected:
    int number;
    long start;
    TGTimeSignature *timeSignature;
    TGTempo *tempo;
    TGMarker *marker;
    bool repeatOpen;
    int repeatAlternative;
    int repeatClose;
    int tripletFeel;
    TGSong *song;
    
public:
    TGMeasureHeader(TGFactory *factory){
        number = 0;
        start = TGDuration_QUARTER_TIME;
        timeSignature = factory->newTimeSignature();
        tempo = factory->newTempo();
        marker = NULL;
        tripletFeel = TGMeasureHeader_TRIPLET_FEEL_NONE;
        repeatOpen = false;
        repeatClose = 0;
        repeatAlternative = 0;
        checkMarker();
    }
    
    int getNumber() {
        return number;
    }
    
    void setNumber(int _number) {
        number = _number;
        checkMarker();
    }
    
    int getRepeatClose() {
        return repeatClose;
    }
    
    void setRepeatClose(int _repeatClose) {
        repeatClose = _repeatClose;
    }
    
    int getRepeatAlternative() {
        return repeatAlternative;
    }
    
    /**
     * bitwise value 1 TO 8.
     * (1 << AlternativeNumber)
     */
    void setRepeatAlternative(int _repeatAlternative) {
        repeatAlternative = _repeatAlternative;
    }
    
    bool isRepeatOpen() {
        return repeatOpen;
    }
    
    void setRepeatOpen(bool _repeatOpen) {
        repeatOpen = _repeatOpen;
    }
    
    long getStart() {
        return start;
    }
    
    void setStart(long _start) {
        start = _start;
    }
    
    int getTripletFeel() {
        return tripletFeel;
    }
    
    void setTripletFeel(int _tripletFeel) {
        tripletFeel = _tripletFeel;
    }
    
    TGTempo* getTempo() {
        return tempo;
    }
    
    void setTempo(TGTempo* aTempo) {
        tempo = aTempo;
    }
    
    TGTimeSignature* getTimeSignature() {
        return timeSignature;
    }
    
    void setTimeSignature(TGTimeSignature* aTimeSignature) {
        timeSignature = aTimeSignature;
    }
    
    TGMarker* getMarker() {
        return marker;
    }
    
    void setMarker(TGMarker *aMarker) {
        marker = aMarker;
    }
    
    bool hasMarker(){
        return (getMarker() != NULL);
    }
    
protected:
    void checkMarker(){
        if(hasMarker()){
            marker->setMeasure(getNumber());
        }
    }
    
public:
    long getLength(){
        return timeSignature->getNumerator() * timeSignature->getDenominator()->getTime();
    }
    
    TGSong *getSong() {
        return song;
    }
    
    void setSong(TGSong *aSong) {
        song = aSong;
    }
    
    void makeEqual(TGMeasureHeader *measure){
        start = measure->getStart();
        timeSignature = measure->getTimeSignature();
        tempo = measure->getTempo();
        marker = measure->getMarker();
        repeatOpen = measure->isRepeatOpen();
        repeatClose = measure->getRepeatClose();
        repeatAlternative = measure->getRepeatAlternative();
        checkMarker();
    }
    
    TGMeasureHeader *clone(TGFactory *factory){
        TGMeasureHeader *header = factory->newHeader();
        header->setNumber(getNumber());
        header->setStart(getStart());
        header->setRepeatOpen(isRepeatOpen());
        header->setRepeatAlternative(getRepeatAlternative());
        header->setRepeatClose(getRepeatClose());
        header->setTripletFeel(getTripletFeel());
        getTimeSignature()->copy(header->getTimeSignature());
        getTempo()->copy(header->getTempo());
        header->setMarker(hasMarker()?getMarker()->clone(factory):NULL);
        return header;
    }
};

#endif

