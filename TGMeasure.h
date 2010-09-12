#ifndef TG_MEASURE_H
#define TG_MEASURE_H

#include "TGFactory.h"
#include "TGMeasureHeader.h"
#include "TGTrack.h"

#define TGMeasure_CLEF_TREBLE  1
#define TGMeasure_CLEF_BASS  2
#define TGMeasure_CLEF_TENOR  3
#define TGMeasure_CLEF_ALTO  4

#define TGMeasure_DEFAULT_CLEF TGMeasure_CLEF_TREBLE;
#define TGMeasure_DEFAULT_KEY_SIGNATURE 0

class TGMeasure 
{

protected:
    TGMeasureHeader *header;
    TGTrack *track;
    int clef;
    int keySignature;
    
    TGBeatList beats;
    
public:
    TGMeasure(TGMeasureHeader* aHeader);
    virtual ~TGMeasure();
    
    TGTrack* getTrack() {
        return track;
    }
    
    void setTrack(TGTrack* aTrack) {
        track = aTrack;
    }
    
    int getClef() {
        return clef;
    }
    
    void setClef(int _clef) {
        clef = _clef;
    }
    
    int getKeySignature() {
        return keySignature;
    }
    
    void setKeySignature(int _keySignature) {
        keySignature = _keySignature;
    }
    
    TGBeatList& getBeats() {
        return beats;
    }
    
    void addBeat(TGBeat *beat);
    void moveBeat(int index, TGBeat* beat);
    void removeBeat(TGBeat *beat);
    TGBeat *getBeat(int index);
    int countBeats();

    TGMeasureHeader *getHeader() {
        return header;
    }
    
    void setHeader(TGMeasureHeader *aHeader) {
        header = aHeader;
    }
    
    int getNumber() {
        return header->getNumber();
    }
    
    int getRepeatClose() {
        return header->getRepeatClose();
    }
    
    long getStart() {
        return header->getStart();
    }
    
    TGTempo *getTempo() {
        return header->getTempo();
    }
    
    TGTimeSignature *getTimeSignature() {
        return header->getTimeSignature();
    }
    
    bool isRepeatOpen() {
        return header->isRepeatOpen();
    }
    
    int getTripletFeel() {
        return header->getTripletFeel();
    }
    
    long getLength() {
        return header->getLength();
    }
    
    bool hasMarker() {
        return header->hasMarker();
    }
    
    TGMarker *getMarker(){
        return header->getMarker();
    }
    void makeEqual(TGMeasure* measure);
    TGMeasure* clone(TGFactory *factory,TGMeasureHeader *header);
};

#endif
