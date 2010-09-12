#include "TGBeat.h"
#include "TGDuration.h"
#include "TGVoice.h"
#include "TGStroke.h"
#include "TGText.h"
#include "TGChord.h"

TGBeat::TGBeat(TGFactory* factory)
{
    chord = NULL;
    text = NULL;
    measure = NULL;

    start = TGDuration_QUARTER_TIME;
    stroke = factory->newStroke();

    for (int i=0; i<TGBeat_MAX_VOICES; ++i)
    {
        voices[i] = NULL;
	setVoice(i, factory->newVoice(i));
    }
}

TGBeat::~TGBeat()
{
#ifndef AUTOMATIC_DELETE
    for (int i=0; i<TGBeat_MAX_VOICES; ++i)
    {
        delete voices[i]; voices[i] = NULL;
    }
    delete stroke;
    stroke = NULL;

    if (text) delete text;
    if (chord) delete chord;
    text = NULL;
    chord = NULL;
#endif
}

void TGBeat::setVoice(int _index, TGVoice* _voice)
{
    if (_index >= 0 && _index < TGBeat_MAX_VOICES)
    {
        if (voices[_index])
        {
#ifndef AUTOMATIC_DELETE
            delete voices[_index];
#endif
            voices[_index] = NULL;
        }

	voices[_index] = _voice;
	voices[_index]->setBeat(this);
    }
}

TGVoice* TGBeat::getVoice(int _index)
{
    if (_index >= 0 && _index < TGBeat_MAX_VOICES)
    {
	return voices[_index];
    }
    return NULL;
    
}
void TGBeat::setText(TGText* _text)
{
    text = _text;
    text->setBeat(this);
}

bool TGBeat::isRestBeat()
{
    for (int i=0; i<countVoices(); ++i)
    {
	TGVoice* v = getVoice(i);
	if (!v->isEmpty() && !v->isRestVoice() )
	{
	    return false;
	}
    }
    return true;
}

TGBeat* TGBeat::clone(TGFactory* factory)
{
    TGBeat* beat = factory->newBeat();
    beat->setStart(getStart());
    getStroke()->copy( beat->getStroke() );
    for (int i=0; i<TGBeat_MAX_VOICES; ++i)
    {
	beat->setVoice(i, voices[i]->clone(factory));
    }
    if (chord != NULL)
    {
	beat->setChord( chord->clone(factory) );
    }
    if (text != NULL)
    {
	beat->setText( text->clone(factory) );
    }
    return beat;
}

void TGBeat::removeChord() {
#ifndef AUTOMATIC_DELETE
    delete chord; chord = NULL; 
#endif
}
void TGBeat::removeText() {
#ifndef AUTOMATIC_DELETE
    delete text; text = NULL; 
#endif
}
