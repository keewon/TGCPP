#ifndef TG_EFFECT_BEND_H
#define TG_EFFECT_BEND_H

#include "TGFactory.h"

#include <list>

#define TGEffectBend_SEMITONE_LENGTH 1
#define TGEffectBend_MAX_POSITION_LENGTH 12
#define TGEffectBend_MAX_VALUE_LENGTH (SEMITONE_LENGTH * 12)

class BendPoint
{
protected:
    int position;
    int value;


public:
    BendPoint() {
        position = 0;
        value = 0;
    }

    BendPoint(int aPosition,int aValue){
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
        return (duration * getPosition() / TGEffectBend_MAX_POSITION_LENGTH);
    }
};

typedef std::list<BendPoint, dumb_allocator<BendPoint> > BendPointList;

class TGEffectBend 
{
public:
    
protected:
    BendPointList points;
    
public:
    TGEffectBend(){
    }
    
    void addPoint(int position,int value){
        points.push_back(BendPoint(position,value));
    }

    void addPoint(BendPoint& aPoint)
    {
        points.push_back(aPoint);
    }
    
    BendPointList* getPoints(){
        return &points;
    }
    
    TGEffectBend *clone(TGFactory *factory){
        TGEffectBend *effect = factory->newEffectBend();

        for ( BendPointList::iterator i=points.begin(); i != points.end(); ++i)
        {
            effect->addPoint( *i );
        }
        return effect;
    }
    
};


#endif
