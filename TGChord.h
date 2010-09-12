#ifndef TG_CHORD_H
#define TG_CHORD_H

#include "TGFactory.h"

class TGChord 
{
    protected:
	int firstFret;
	int* strings;
	int stringsLength;
	TGStdString name;
	TGBeat* beat;

    public:
	TGChord(int length)
	{
            beat = NULL;

	    stringsLength = length;
	    strings = (int*)das_alloc(sizeof(int)*length);
	    for (int i=0; i<length; ++i)
	    {
		strings[i] = -1;
	    }
	}

	virtual ~TGChord()
	{
            das_free(strings, stringsLength*sizeof(int));
	}

	TGBeat* getBeat() { return beat; }
	void setBeat(TGBeat* _beat) { beat = _beat; }

	void addFretValue(int _string, int _fret)
	{
	    if (_string >= 0 && _string < stringsLength)
	    {
		strings[_string] = _fret;
	    }
	}

	int getFretValue(int _string)
	{
	    if (_string >= 0 && _string < stringsLength)
	    {
		return strings[_string];
	    }
	    return -1;
	}

	int getFirstFret() { return firstFret; }
	void setFirstFret(int _firstFret) { firstFret = _firstFret; }

	int* getStrings() { return strings; }
	void setStrings(int* _strings, int _stringsLength)
	{
	    if (stringsLength != _stringsLength)
	    {
                das_free(strings, stringsLength * sizeof(int));
		strings = NULL;
		
		if (_stringsLength > 0 && _strings)
		{
		    strings = (int*)das_alloc(sizeof(int)*_stringsLength);
		    stringsLength = _stringsLength;
		}
	    }
	    if (_stringsLength > 0 && _strings)
	    {
		for (int i=0; i<_stringsLength; ++i)
		{
		    strings[i] = _strings[i];
		}
	    }
	}

	int countStrings() { return stringsLength; }
	int countNotes()
	{
	    int count = 0;
	    for (int i=0; i<stringsLength;  ++i)
	    {
		if (strings[i] >= 0)
		{
		    count++;
		}
	    }
	    return count;
	}

	TGStdString& getName() {
	    return name;
	}

	void setName(TGStdString _name)
	{
	    name = _name;
	}

	TGChord* clone(TGFactory* factory)
	{
	    TGChord* chord = factory->newChord(stringsLength);
	    chord->setName(getName());
	    chord->setFirstFret(getFirstFret());
	    chord->setStrings(strings, stringsLength);
	    return chord;
	}
    
};

#endif
