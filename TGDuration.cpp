#include "TGDuration.h"
#include "TGTupleto.h"

long TGDuration::getTime() {
    long time = (long)( TGDuration_QUARTER_TIME * ( 4.0f / value ) ) ;
    if(dotted){
	time += time / 2;
    }else if(doubleDotted){
	time += ((time / 4) * 3);
    }
    return tupleto->convertTime(time);
}

void TGDuration::copy(TGDuration* duration){
    duration->setValue(getValue());
    duration->setDotted(isDotted());
    duration->setDoubleDotted(isDoubleDotted());
    getTupleto()->copy(duration->getTupleto());
}

TGDuration* TGDuration::fromTime(TGFactory* factory,long time,TGDuration* minimum, int diff)
{
    TGDuration *duration = minimum->clone(factory);
    TGDuration *tmpDuration = factory->newDuration();
    tmpDuration->setValue(TGDuration_WHOLE);
    tmpDuration->setDotted(true);
    bool finish = false;
    while(!finish){
	long tmpTime = tmpDuration->getTime();
	if(tmpTime - diff <= time){
	    if(abs( tmpTime - time ) < abs( duration->getTime() - time ) ){
		duration = tmpDuration->clone(factory);
	    }
	}
	if(tmpDuration->isDotted()){
	    tmpDuration->setDotted(false);
	}else if(tmpDuration->getTupleto()->isEqualToNormal()){
	    tmpDuration->getTupleto()->setEnters(3);
	    tmpDuration->getTupleto()->setTimes(2);
	}else{
	    tmpDuration->setValue(tmpDuration->getValue() * 2);
	    tmpDuration->setDotted(true);
	    tmpDuration->getTupleto()->setEnters(1);
	    tmpDuration->getTupleto()->setTimes(1);
	}
	if(tmpDuration->getValue() > TGDuration_SIXTY_FOURTH){
	    finish = true;
	}
    }
    return duration;
}
