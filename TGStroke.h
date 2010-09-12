#ifndef TG_STROKE_H
#define TG_STROKE_H

#include "TGFactory.h"

#define TGStroke_STROKE_NONE 0
#define TGStroke_STROKE_UP 1
#define TGStroke_STROKE_DOWN -1

class TGStroke 
{
    protected:
	int direction;
	int value;

    public:
	TGStroke() { direction = TGStroke_STROKE_NONE; }
	int getDirection() { return direction; }
	void setDirection(int _direction) { direction = _direction; }

	int getValue() { return value; }
	void setValue(int aValue) { value = aValue; }

	int getIncrementTime(TGBeat *beat);

	TGStroke* clone(TGFactory* factory)
	{
	    TGStroke* stroke = factory->newStroke();
	    copy(stroke);
	    return stroke;
	}

	void copy(TGStroke* aStroke)
	{
	    aStroke->setValue(getValue());
	    aStroke->setDirection(getDirection());
	}
};

#endif
