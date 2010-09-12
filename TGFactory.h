#ifndef TG_FACTORY_H
#define TG_FACTORY_H

#define AUTOMATIC_DELETE 1

#include <cstdlib>
#include <vector>
#include <string>
#include "DumbAllocator.h"

class TGBeat;
class TGChannel;
class TGChord;
class TGDuration;
class TGLyric;
class TGMarker;
class TGMeasure;
class TGMeasureHeader;
class TGNote;
class TGNoteEffect;
class TGScale;
class TGSong;
class TGString;
class TGStroke;
class TGTempo;
class TGText;
class TGTimeSignature;
class TGTrack;
class TGTupleto;
class TGVelocities;
class TGVoice;

class TGEffectBend;
class TGEffectTremoloBar;
class TGEffectTremoloPicking;
class TGEffectGrace;
class TGEffectHarmonic;
class TGEffectTrill;

typedef std::vector<TGChannel*, dumb_allocator<TGChannel*> > TGChannelList;
typedef std::vector<TGBeat*, dumb_allocator<TGBeat*> > TGBeatList;
typedef std::vector<TGTrack*, dumb_allocator<TGTrack*> > TGTrackList;
typedef std::vector<TGMeasure*, dumb_allocator<TGMeasure*> > TGMeasureList;
typedef std::vector<TGMeasureHeader*, dumb_allocator<TGMeasureHeader*> > TGMeasureHeaderList;
typedef std::vector<TGNote*, dumb_allocator<TGNote*> > TGNoteList;
typedef std::vector<TGString*, dumb_allocator<TGString*> > TGStringList;

typedef std::basic_string<char, std::char_traits<char>, dumb_allocator<char> > TGStdString;

enum
{
    PRODUCT_TGSong=0,
    PRODUCT_TGLyric,
    PRODUCT_TGMarker,
    PRODUCT_TGChord,
    PRODUCT_TGScale,
    PRODUCT_TGTupleto,
    PRODUCT_TGDuration,
    PRODUCT_TGTimeSignature,
    PRODUCT_TGTempo,
    PRODUCT_TGChannel,
    PRODUCT_TGTrack,
    PRODUCT_TGMeasureHeader,
    PRODUCT_TGMeasure,
    PRODUCT_TGBeat,
    PRODUCT_TGVoice,
    PRODUCT_TGNote,
    PRODUCT_TGString,
    PRODUCT_TGStroke,
    PRODUCT_TGText,
    PRODUCT_TGNoteEffect,
    PRODUCT_TGEffectBend,
    PRODUCT_TGEffectTremoloBar,
    PRODUCT_TGEffectGrace,
    PRODUCT_TGEffectHarmonic,
    PRODUCT_TGEffectTrill,
    PRODUCT_TGEffectTremoloPicking,
    PRODUCT_TGChannelList,

    PRODUCT_MAX,
};

class TGFactory {
public:
    TGFactory() {
        for (int i=0; i<PRODUCT_MAX; ++i)
        {
            newCount[i] = 0;
        }
    }
    ~TGFactory();

    static int newCount[PRODUCT_MAX];
    
    TGSong *newSong();
    TGLyric *newLyric();
    TGMarker *newMarker();
    TGChord *newChord(int length);
    TGScale *newScale();

    TGTupleto *newTupleto();
    static TGTupleto *newTupleto2();
    TGDuration *newDuration();
    TGTimeSignature *newTimeSignature();
    TGTempo *newTempo();
    TGChannel *newChannel();
    TGTrack *newTrack();
    TGMeasureHeader *newHeader();
    TGMeasure *newMeasure(TGMeasureHeader *header);
    TGBeat *newBeat();
    TGVoice *newVoice(int index);
    TGNote *newNote();
    TGString *newString();
    TGStroke *newStroke();
    TGText *newText();
    TGNoteEffect *newEffect();
    TGEffectBend *newEffectBend();
    TGEffectTremoloBar *newEffectTremoloBar();
    TGEffectGrace *newEffectGrace();
    TGEffectHarmonic *newEffectHarmonic();
    TGEffectTrill *newEffectTrill();
    TGEffectTremoloPicking *newEffectTremoloPicking();

    TGChannelList *newChannelList();
};

#endif
