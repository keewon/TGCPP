#include "TGTrack.h"
#include "TGChannel.h"
#include "TGMeasure.h"
#include "TGLyric.h"
#include "TGString.h"

TGTrack::TGTrack(TGFactory *factory) {
    number = 0;
    offset = 0;
    solo = false;
    mute = false;
    channel = factory->newChannel();
    color = COLOR_BLACK;
    lyrics = factory->newLyric();
    song = NULL;
}

TGTrack::~TGTrack()
{
    clear();
#ifndef AUTOMATIC_DELETE
    delete channel;
    delete lyrics;
#endif
}

bool TGTrack::isPercussionTrack(){
    return (getChannel()->isPercussionChannel());
}

void TGTrack::addMeasure(TGMeasure *measure){
    measure->setTrack(this);
    measures.push_back(measure);
}

void TGTrack::addMeasure(int index, TGMeasure* aMeasure){
    aMeasure->setTrack(this);
    measures.insert(measures.begin() + index, aMeasure);
}

void TGTrack::removeMeasure(int index){
    TGMeasureList::iterator i = measures.begin() + index;
    if (i != measures.end())
        measures.erase( i );
}

TGTrack * TGTrack::clone(TGFactory *factory,TGSong *song){
    TGTrack *track = factory->newTrack();
    copy(factory, song, track);
    return track;
}

void  TGTrack::copy(TGFactory *factory,TGSong *song,TGTrack *track){
    track->clear();
    track->setNumber(getNumber());
    track->setName(getName());
    track->setOffset(getOffset());
    getChannel()->copy(track->getChannel());
    color = track->getColor();
    getLyrics()->copy(track->getLyrics());

    for (TGStringList::iterator i=strings.begin(); i!=strings.end(); ++i)
    {
        TGString *s = (*i);
        track->getStrings().push_back(s->clone(factory));
    }
    for (int i = 0; i < countMeasures(); i++) {
        TGMeasure *measure = getMeasure(i);
        track->addMeasure(measure->clone(factory,song->getMeasureHeader(i)));
    }
}

void TGTrack::clear(){
#ifndef AUTOMATIC_DELETE
    for (TGStringList::iterator i = strings.begin(); i!=strings.end(); ++i)
    {
        delete (*i);
    }
#endif
    strings.clear();

#ifndef AUTOMATIC_DELETE
    for (TGMeasureList::iterator i = measures.begin(); i!=measures.end(); ++i)
    {
        delete (*i);
    }
#endif
    measures.clear();
}
