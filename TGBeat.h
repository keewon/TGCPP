#ifndef TG_BEAT_H
#define TG_BEAT_H

#include "TGFactory.h"

#define TGBeat_MAX_VOICES 2

class TGBeat 
{
    protected:
	long start;
	TGMeasure* measure;
	TGChord* chord;
	TGText* text;
	TGVoice* voices[TGBeat_MAX_VOICES];
	TGStroke* stroke;
    public:
	TGBeat(TGFactory* factory);
	virtual ~TGBeat();

	TGMeasure* getMeasure() { return measure; }
	void setMeasure(TGMeasure* _measure) { measure = _measure; }

	long getStart() { return start; }
	void setStart(long _start) { start = _start; }

	void setVoice(int _index, TGVoice* _voice);
	TGVoice* getVoice(int _index);
	int countVoices() { return TGBeat_MAX_VOICES; }

	void setChord(TGChord* aChord) { chord = aChord; }
	TGChord* getChord() { return chord; }
	void removeChord();

	TGText* getText() { return text; }
	void setText(TGText* _text);
	void removeText();

	bool isChordBeat() { return chord != NULL; }
	bool isTextBeat() { return text != NULL; }

	TGStroke* getStroke() { return stroke; }
	bool isRestBeat();

	TGBeat* clone(TGFactory* factory);
};

#endif
