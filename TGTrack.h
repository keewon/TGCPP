#ifndef TG_TRACK_H
#define TG_TRACK_H

#include "TGFactory.h"
#include "TGColor.h"

class TGTrack 
{
public:
    const static int MAX_OFFSET = 24;
    const static int MIN_OFFSET = -24;
    
protected:
    int number;
    int offset;
    bool solo;
    bool mute;
    TGStdString name;
    TGMeasureList measures;
    TGStringList strings;
    TGChannel *channel;
    TGColor color;
    TGLyric *lyrics;
    TGSong *song;
    
public:
    TGTrack(TGFactory *factory);
    virtual ~TGTrack();
    
    int getNumber() {
        return number;
    }
    
    void setNumber(int aNumber) {
        number = aNumber;
    }
    
    void addMeasure(TGMeasure *measure);
    void addMeasure(int index, TGMeasure* measure);
    
    TGMeasure *getMeasure(int index){
        if(index >= 0 && index < countMeasures()){
            return measures[index];
        }
        return NULL;
    }
    
    void removeMeasure(int index);
    
    int countMeasures(){
        return measures.size();
    }
    
    TGChannel *getChannel() {
        return channel;
    }
    
    void setChannel(TGChannel *aChannel) {
        channel = aChannel;
    }
    
    TGStringList& getStrings() {
        return strings;
    }
    
    TGColor getColor() {
        return color;
    }
    
    void setColor(TGColor aColor) {
        color = aColor;
    }
    
    TGStdString& getName() {
        return name;
    }
    
    void setName(TGStdString aName) {
        name = aName;
    }
    
    int getOffset() {
        return offset;
    }
    
    void setOffset(int aOffset) {
        offset = aOffset;
    }
    
    bool isSolo() {
        return solo;
    }
    
    void setSolo(bool aSolo) {
        solo = aSolo;
    }
    
    bool isMute() {
        return mute;
    }
    
    void setMute(bool aMute) {
        mute = aMute;
    }
    
    TGLyric *getLyrics() {
        return lyrics;
    }
    
    void setLyrics(TGLyric *aLyrics) {
        lyrics = aLyrics;
    }
    
    TGString *getString(int number){
        return strings[number - 1];
    }
    
    int stringCount(){
        return strings.size();
    }
    
    bool isPercussionTrack();
    
    TGSong *getSong() {
        return song;
    }
    
    void setSong(TGSong *aSong) {
        song = aSong;
    }
    
    void clear();
    
    TGTrack *clone(TGFactory *factory,TGSong *song);
    void copy(TGFactory *factory,TGSong *song,TGTrack *track);
    
};

#endif
