#include "GP3InputStream.h"
#include "GP4InputStream.h"
#include "GP5InputStream.h"
#include "TGFactory.h"
#include <fstream>
#include "TGBeat.h"
#include "TGStroke.h"
#include "TGChord.h"
#include "TGVoice.h"
#include <cstdio>
#include "DAS.h"

void showSong(TGSong* song)
{
    for (int i=0; i<song->countTracks(); ++i)
    {
        TGTrack* t = song->getTrack(i);

        printf("track name: %s\n", t->getName().c_str());
        printf("offset: %d\n", t->getOffset());
        printf("num of measures: %d\n", t->countMeasures());
        printf("string count: %d\n", t->stringCount());

        for (int j=0; j<t->countMeasures(); ++j)
        {
            printf(" measure %d\n", j);
            TGMeasure* m=t->getMeasure(j);
            for (int k=0; k<m->countBeats(); ++k)
            {
                printf("  beat %d\n", k);
                TGBeat* b = m->getBeat(k);
                TGStroke* s = b->getStroke();
                printf("   stroke: dir=%d v=%d\n", s->getDirection(), s->getValue());
                TGChord* c = b->getChord();
                if (c)
                {
                    printf("   chord: firstFret=%d stringsCount=%d\n", c->getFirstFret(), c->countStrings());
                    for (int l=0; l<c->countStrings(); ++l)
                    {
                        printf("    string: %d\n", c->getStrings()[l]);
                    }
                }
                TGVoice* v0 = b->getVoice(0);
                //TGVoice* v1 = b->getVoice(1);

                TGDuration* d0 = v0->getDuration();
                //TGDuration* d1 = v1->getDuration();

                for (int l=0; l<v0->countNotes(); ++l)
                {
                    TGNote* n = v0->getNote(l);
                    printf("     v0 note: %d\n", n->getValue());
                }
                printf("     duration: %d\n", d0->getValue());
                
            }
        }
    }
}

int main()
{
    DumbAllocationSystem d;
    bool b = d.initialize(1024*1024*4, 1024*1024);
    if (!b)
    {
        printf("das init failed\n");
        return -1;
    }

    das_set_global(&d);

if (1)
    {
        TGFactory factory;
        GP3InputStream a;

        std::ifstream f;

        printf("highway_star.gp3\n");

        f.open("highway_star.gp3", std::ios::in);

        a.init(&factory, &f);
        TGSong* song = a.readSong();
        showSong(song);

 //       getc(stdin);
    }

    d.reset();

if (1)
    {
        TGFactory factory;
        GP4InputStream a;

        std::ifstream f;

        printf("god_knows.gp4\n");

        f.open("god_knows.gp4", std::ios::in);

        a.init(&factory, &f);
        TGSong* song = a.readSong();

//        getc(stdin);
        showSong(song);
    }

    d.reset();

if (1)
    {
        TGFactory factory;
        GP5InputStream a;

        std::ifstream f;

        printf("smoke.gp5\n");

        f.open("smoke.gp5", std::ios::in);

        a.init(&factory, &f);
        TGSong* song = a.readSong();

        showSong(song);
        //getc(stdin);
    }

    d.reset();
    d.finalize();

    return 0;
}
