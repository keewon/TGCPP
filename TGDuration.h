#ifndef TG_DURATION_H
#define TG_DURATION_H

#include "TGFactory.h"
#include "TGTupleto.h"
#include <cmath>

#define TGDuration_QUARTER_TIME 960
#define TGDuration_WHOLE  1
#define TGDuration_HALF  2
#define TGDuration_QUARTER  4
#define TGDuration_EIGHTH  8
#define TGDuration_SIXTEENTH  16
#define TGDuration_THIRTY_SECOND  32
#define TGDuration_SIXTY_FOURTH  64

class TGDuration 
{
    protected:
	int value;
	TGTupleto* tupleto;
	bool dotted;
	bool doubleDotted;
    public:
	TGDuration(TGFactory* factory)
	{
	    value = TGDuration_QUARTER;
	    dotted = false;
	    doubleDotted = false;
	    tupleto = factory->newTupleto();
	}

	virtual ~TGDuration()
	{
#ifndef AUTOMATIC_DELETE
	    delete tupleto;
#endif
	}

	int getValue() { return value; }
	void setValue(int _value) { value = _value; }

	bool isDotted() { return dotted; }
	void setDotted(bool _dotted) { dotted = _dotted; }

	bool isDoubleDotted() { return doubleDotted; }
	void setDoubleDotted(bool _d) { doubleDotted = _d; }

	TGTupleto* getTupleto() { return tupleto; }

	long getTime();

	static TGDuration* fromTime(TGFactory* factory,long time){
	    TGDuration *duration = factory->newDuration();
	    duration->setValue(TGDuration_SIXTY_FOURTH);
	    duration->setDotted(false);
	    duration->setDoubleDotted(false);
	    duration->getTupleto()->setEnters(3);
	    duration->getTupleto()->setTimes(2);
	    return fromTime(factory,time,duration);
	}
    
	static TGDuration* fromTime(TGFactory* factory,long time,TGDuration* minDuration)
	{
	    return fromTime(factory, time, minDuration, 10);
	}
    
	static TGDuration* fromTime(TGFactory* factory,long time,TGDuration* minimum, int diff);

	int getIndex(){
	    int index = 0;
	    int value = value;
	    while( ( value = ( value >> 1 ) ) > 0 ){
		index ++;
	    }
	    return index;
	}
    
	bool isEqual(TGDuration* d){
	    return (getValue() == d->getValue() && isDotted() == d->isDotted() && isDoubleDotted() == d->isDoubleDotted() && getTupleto()->isEqual(d->getTupleto()));
	}
    
	TGDuration* clone(TGFactory* factory){
	    TGDuration* duration = factory->newDuration();
	    copy(duration);
	    return duration;
	}
    
	void copy(TGDuration* duration);
};

#endif
