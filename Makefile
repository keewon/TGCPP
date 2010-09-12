.SUFFIXES: .cpp .o

CC = g++
CFLAGS = -c -Wall -g
SRCS_TG = TGBeat.cpp TGDuration.cpp TGFactory.cpp TGMeasure.cpp TGLyric.cpp TGNoteEffect.cpp TGSong.cpp TGStroke.cpp TGTimeSignature.cpp TGTrack.cpp TGVoice.cpp DAS.cpp

SRCS_GP = GTPInputStream.cpp GP3InputStream.cpp GP4InputStream.cpp GP5InputStream.cpp
SRCS_ETC = MidiPercussion.cpp

OBJS_TG = $(SRCS_TG:.cpp=.o)
OBJS_GP = $(SRCS_GP:.cpp=.o)
OBJS_ETC = $(SRCS_ETC:.cpp=.o)

all: $(OBJS_TG) $(OBJS_GP) $(OBJS_ETC) main.o
	g++ -g -Wall $(OBJS_TG) $(OBJS_GP) $(OBJS_ETC) main.o

clean:
	rm *.o a.out

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
