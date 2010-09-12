#include "TGFactory.h"

#include "TGBeat.h"
#include "TGChannel.h"
#include "TGChord.h"
#include "TGColor.h"
#include "TGDuration.h"
#include "TGLyric.h"
#include "TGMarker.h"
#include "TGMeasure.h"
#include "TGMeasureHeader.h"
#include "TGNote.h"
#include "TGNoteEffect.h"
#include "TGScale.h"
#include "TGSong.h"
#include "TGString.h"
#include "TGStroke.h"
#include "TGTempo.h"
#include "TGText.h"
#include "TGTimeSignature.h"
#include "TGTrack.h"
#include "TGTupleto.h"
#include "TGVoice.h"
#include "TGEffectBend.h"
#include "TGEffectGrace.h"
#include "TGEffectHarmonic.h"
#include "TGEffectTremoloBar.h"
#include "TGEffectTremoloPicking.h"
#include "TGEffectTrill.h"

#include <cstdio>

/*
#define RETURN_NEW(C) { C* c=new C; newCount[PRODUCT_##C]++; return c; }
#define RETURN_NEW_WITH_ARG(C, i) { C* c=new C(i); newCount[PRODUCT_##C]++; return c; }
#define RETURN_NEW_WITH_TWO_ARG(C, a, b) { C* c=new C(a, b); newCount[PRODUCT_##C]++; return c; }
*/
#define RETURN_NEW(C) { void* buf = das_alloc(sizeof(C)); C* c=new(buf)C; newCount[PRODUCT_##C]++; return c; }
#define RETURN_NEW_WITH_ARG(C, i) { void* buf = das_alloc(sizeof(C)); C* c=new(buf)C(i); newCount[PRODUCT_##C]++; return c; }
#define RETURN_NEW_WITH_TWO_ARG(C, a, b) { void* buf = das_alloc(sizeof(C)); C* c=new(buf)C(a,b); newCount[PRODUCT_##C]++; return c; }

int TGFactory::newCount[PRODUCT_MAX];

TGFactory::~TGFactory() {
    printf("TGFactory new counts\n");
    for (int i=0; i<PRODUCT_MAX; ++i)
    {
        printf("%d\n", newCount[i]);
    }
}

TGSong* TGFactory::newSong(){
    RETURN_NEW(TGSong);
}
    
TGLyric * TGFactory::newLyric(){
    RETURN_NEW(TGLyric);
}
    
TGMarker* TGFactory::newMarker(){
    RETURN_NEW_WITH_ARG(TGMarker, this);
}
    
TGChord * TGFactory::newChord(int length){
    RETURN_NEW_WITH_ARG(TGChord, length);
}
    
TGScale * TGFactory::newScale(){
    RETURN_NEW(TGScale);
}

TGTupleto * TGFactory::newTupleto(){
    RETURN_NEW(TGTupleto);
}

TGTupleto * TGFactory::newTupleto2(){
    RETURN_NEW(TGTupleto);
}

TGDuration * TGFactory::newDuration(){
    RETURN_NEW_WITH_ARG(TGDuration, this);
}

TGTimeSignature * TGFactory::newTimeSignature(){
    RETURN_NEW_WITH_ARG(TGTimeSignature, this);
}

TGTempo * TGFactory::newTempo(){
    RETURN_NEW(TGTempo);
}

TGChannel * TGFactory::newChannel(){
    RETURN_NEW(TGChannel);
}

TGTrack * TGFactory::newTrack(){
    RETURN_NEW_WITH_ARG(TGTrack, this);
}

TGMeasureHeader * TGFactory::newHeader(){
    RETURN_NEW_WITH_ARG(TGMeasureHeader, this);
}

TGMeasure * TGFactory::newMeasure(TGMeasureHeader *header){
    RETURN_NEW_WITH_ARG(TGMeasure, header);
}

TGBeat * TGFactory::newBeat(){
    RETURN_NEW_WITH_ARG(TGBeat, this);
}

TGVoice * TGFactory::newVoice(int index){
    RETURN_NEW_WITH_TWO_ARG(TGVoice, this, index);
}

TGNote * TGFactory::newNote(){
    RETURN_NEW_WITH_ARG(TGNote, this);
}

TGString * TGFactory::newString(){
    RETURN_NEW(TGString);
}

TGStroke * TGFactory::newStroke(){
    RETURN_NEW(TGStroke);
}

TGText * TGFactory::newText(){
    RETURN_NEW(TGText);
}

TGNoteEffect * TGFactory::newEffect(){
    RETURN_NEW(TGNoteEffect);
}

TGEffectBend * TGFactory::newEffectBend(){
    RETURN_NEW(TGEffectBend);
}

TGEffectTremoloBar * TGFactory::newEffectTremoloBar(){
    RETURN_NEW(TGEffectTremoloBar);
}

TGEffectGrace * TGFactory::newEffectGrace(){
    RETURN_NEW(TGEffectGrace);
}

TGEffectHarmonic * TGFactory::newEffectHarmonic(){
    RETURN_NEW(TGEffectHarmonic);
}

TGEffectTrill * TGFactory::newEffectTrill(){
    RETURN_NEW_WITH_ARG(TGEffectTrill, this);
}

TGEffectTremoloPicking * TGFactory::newEffectTremoloPicking(){
    RETURN_NEW_WITH_ARG(TGEffectTremoloPicking, this);
}

TGChannelList* TGFactory::newChannelList(){
    RETURN_NEW(TGChannelList);
}
