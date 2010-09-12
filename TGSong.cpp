#include "TGSong.h"
#include "TGMeasure.h"
#include "TGTrack.h"

#include <algorithm>

void TGSong::addMeasureHeader(TGMeasureHeader *measureHeader){
    addMeasureHeader(countMeasureHeaders(),measureHeader);
}

void TGSong::addMeasureHeader(int index,TGMeasureHeader *measureHeader){
    measureHeader->setSong(this);
    measureHeaders.insert(measureHeaders.begin() + index, measureHeader);
}

void TGSong::removeMeasureHeader(int index){
    TGMeasureHeaderList::iterator i = measureHeaders.begin() + index;

    if (i!=measureHeaders.end())
    {
        measureHeaders.erase( i );
#ifndef AUTOMATIC_DELETE
        delete *i;
#endif
    }
}

void TGSong::removeMeasureHeader(TGMeasureHeader *aMeasureHeader)
{
    TGMeasureHeaderList::iterator i = std::find(
        measureHeaders.begin(), measureHeaders.end(), aMeasureHeader);

    if (i!=measureHeaders.end())
    {
        measureHeaders.erase(i);
#ifndef AUTOMATIC_DELETE
        delete *i;
#endif
    }
}

TGMeasureHeader* TGSong::getMeasureHeader(int index){
    return measureHeaders.at(index);
}

int TGSong::countTracks(){
    return tracks.size();
}

void TGSong::addTrack(TGTrack *aTrack){
    addTrack(countTracks(), aTrack);
}

void TGSong::addTrack(int index,TGTrack *aTrack){
    aTrack->setSong(this);
    tracks.insert(tracks.begin() + index, aTrack);
}

void TGSong::moveTrack(int index,TGTrack *aTrack){
    TGTrackList::iterator i = std::find(tracks.begin(), tracks.end(), aTrack);
    if (i != tracks.end())
    {
        tracks.erase(i);
    }
    tracks.insert(tracks.begin() + index, aTrack);
}

void TGSong::removeTrack(TGTrack *aTrack){
    TGTrackList::iterator i = std::find(tracks.begin(), tracks.end(), aTrack);
    if (i != tracks.end())
    {
        tracks.erase(i);
    }
    aTrack->clear();
#ifndef AUTOMATIC_DELETE
    delete aTrack;
#endif
}

TGTrack* TGSong::getTrack(int index){
    return tracks.at(index);
}

TGTrackList::iterator TGSong::getTrackIterator()
{
    return tracks.begin();
}

TGMeasureHeaderList::iterator TGSong::getMeasureHeaderIterator()
{
    return measureHeaders.begin();
}

TGMeasureHeaderList::iterator TGSong::getMeasureHeaderIteratorEnd()
{
    return measureHeaders.end();
}

bool TGSong::isEmpty(){
    return (countMeasureHeaders() == 0 || countTracks() == 0);
}

void TGSong::clear(){
#ifndef AUTOMATIC_DELETE
    for (TGTrackList::iterator i=tracks.begin(); i!=tracks.end(); ++i)
    {
        delete (*i);
    }

    for (TGMeasureHeaderList::iterator i=measureHeaders.begin(); i!=measureHeaders.end(); ++i)
    {
        delete (*i);
    }
#endif

    tracks.clear();
    measureHeaders.clear();
}

TGSong *TGSong::clone(TGFactory *factory){
    TGSong *song = factory->newSong();
    copy(factory,song);
    return song;
}

void TGSong::copy(TGFactory* factory,TGSong *song){
    song->clear();
    song->setName(getName());
    song->setArtist(getArtist());
    song->setAlbum(getAlbum());
    song->setAuthor(getAuthor());

    std::copy(measureHeaders.begin(), measureHeaders.end(), song->getMeasureHeaderIterator());
    std::copy(tracks.begin(), tracks.end(), song->getTrackIterator());
}
