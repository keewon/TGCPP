#ifndef TG_SONG_H
#define TG_SONG_H

#include "TGFactory.h"

class TGSong 
{

protected:
    TGStdString name;
    TGStdString artist;
    TGStdString album;
    TGStdString author;

    TGTrackList tracks;
    TGMeasureHeaderList measureHeaders;
    
public:
    TGSong() {
    }

    virtual ~TGSong() {
        clear();
    }
    
    TGStdString& getName() {
        return name;
    }
    
    void setName(TGStdString aName) {
        name = aName;
    }
    
    TGStdString& getAlbum() {
        return album;
    }
    
    void setAlbum(TGStdString aAlbum) {
        album = aAlbum;
    }
    
    TGStdString& getAuthor() {
        return author;
    }
    
    void setAuthor(TGStdString aAuthor) {
        author = aAuthor;
    }
    
    TGStdString& getArtist() {
        return artist;
    }
    
    void setArtist(TGStdString aArtist) {
        artist = aArtist;
    }
    
    int countMeasureHeaders(){
        return measureHeaders.size();
    }
    
    void addMeasureHeader(TGMeasureHeader *measureHeader);
    void addMeasureHeader(int index,TGMeasureHeader *measureHeader);
    void removeMeasureHeader(int index);
    void removeMeasureHeader(TGMeasureHeader *measureHeader);
    TGMeasureHeader* getMeasureHeader(int index);
    int countTracks();
    void addTrack(TGTrack *aTrack);
    void addTrack(int index,TGTrack *aTrack);
    void moveTrack(int index,TGTrack *aTrack);
    void removeTrack(TGTrack *aTrack);
    TGTrack* getTrack(int index);
    bool isEmpty();
    void clear();

    TGMeasureHeaderList::iterator getMeasureHeaderIterator();
    TGMeasureHeaderList::iterator getMeasureHeaderIteratorEnd();
    TGTrackList::iterator getTrackIterator();
    
    TGSong *clone(TGFactory *factory);
    void copy(TGFactory* factory,TGSong *song);
};

#endif

