#ifndef TG_TEXT_H
#define TG_TEXT_H

#include "TGFactory.h"

class TGText 
{
protected:
    TGStdString value;
    TGBeat* beat;
public:
    TGText() { beat=NULL; }
    TGBeat* getBeat() { return beat; }
    void setBeat(TGBeat* _beat) { beat = _beat; }
    TGStdString& getValue() { return value; }
    void setValue(TGStdString _value) { value = _value; }
    bool isEmpty() { return value.length() == 0; }
    void copy(TGText* _text) { _text->setValue( getValue() ); }
    TGText* clone(TGFactory* factory)
    {
	TGText* text = factory->newText();
	copy(text);
	return text;
    }
};

#endif
