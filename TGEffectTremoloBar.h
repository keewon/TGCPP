#ifndef TG_EFFECT_TREMOLO_BAR_H
#define TG_EFFECT_TREMOLO_BAR_H

#include "TGFactory.h"
#include <list>

#define TGEffectTremoloBar_MAX_POSITION_LENGTH 12
#define TGEffectTremoloBar_MAX_VALUE_LENGTH 12

class TremoloBarPoint{
protected:
    int position;
    int value;

public:
    TremoloBarPoint(int aPosition,int aValue){
        position = aPosition;
        value = aValue;
    }

    int getPosition() {
        return position;
    }

    int getValue() {
        return value;
    }

    long getTime(long duration){
        return (duration * getPosition() / TGEffectTremoloBar_MAX_POSITION_LENGTH);
    }

};

typedef std::list<TremoloBarPoint, dumb_allocator<TremoloBarPoint> > TremoloBarPointList;

class TGEffectTremoloBar {
public:
    
protected:
    TremoloBarPointList points;
    
public:
    TGEffectTremoloBar(){
    }
    
    void addPoint(int position,int value){
        points.push_back(TremoloBarPoint(position,value));
    }
    
    void addPoint(TremoloBarPoint& aPoint)
    {
        points.push_back(aPoint);
    }
    
    TremoloBarPointList& getPoints(){
        return points;
    }
    
    TGEffectTremoloBar *clone(TGFactory *factory){
        TGEffectTremoloBar *effect = factory->newEffectTremoloBar();

        for (TremoloBarPointList::iterator i=points.begin(); i!=points.end(); ++i)
        {
            effect->addPoint( *i );
        }
        
        return effect;
    }
    
};

#endif
