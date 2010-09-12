#include "TGStroke.h"
#include "TGBeat.h"
#include "TGVoice.h"
#include "TGDuration.h"

int TGStroke::getIncrementTime(TGBeat* beat)
{
    long duration = 0;
    if( value > 0 ){
	for(int v = 0; v < beat->countVoices(); v ++){
	    TGVoice *voice = beat->getVoice( v );
	    if( !voice->isEmpty() ){
		long currentDuration = voice->getDuration()->getTime();
		if(duration == 0 || currentDuration < duration){
		    duration = ( currentDuration <= TGDuration_QUARTER_TIME ? currentDuration : TGDuration_QUARTER_TIME );
		}
	    }
	}
	if( duration > 0 ){
	    // round
	    return int( ( ( duration / 8.0f ) * ( 4.0f / value ) ) + 0.5f );
	}
    }
    return 0;
}

