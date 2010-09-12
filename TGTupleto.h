#ifndef TG_TUPLE_TO
#define TG_TUPLE_TO

#include "TGFactory.h"

class TGTupleto 
{
    protected:
	int enters;
	int times;


    public:
	TGTupleto() { enters = 1; times = 1; }
	int getEnters() { return enters; }
	void setEnters(int _enters) { enters = _enters; }
	int getTimes() { return times; }
	void setTimes(int _times) { times = _times; }
	long convertTime(long _time) { return _time * times / enters; }

	bool isEqual(TGTupleto* tupleto)
	{
	    return (tupleto->getEnters() == getEnters() && 
		    tupleto->getTimes() == getTimes());
	}

        bool isEqualToNormal()
        {
            return enters == 1 && times == 1;
        }

	TGTupleto *clone(TGFactory *factory){
	    TGTupleto *tupleto = factory->newTupleto();
	    copy(tupleto);
	    return tupleto;
	}

	void copy(TGTupleto *tupleto){
	    tupleto->setEnters(enters);
	    tupleto->setTimes(times);
	}

	static TGTupleto* newTupleto(int enters,int times){
	    TGTupleto *tupleto = TGFactory::newTupleto2();
	    tupleto->setEnters(enters);
	    tupleto->setTimes(times);
	    return tupleto;
	}

};

#endif
