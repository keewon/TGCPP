#ifndef TG_COLOR_H
#define TG_COLOR_H

typedef int TGColor;

#define SET_COLOR(r,g,b) ((unsigned char)r<<16 | (unsigned char)g<<8 | (unsigned char)b)
#define COLOR_R(c) (unsigned char)(c >>16)
#define COLOR_G(c) (unsigned char)((c >>8) & 0xff)
#define COLOR_B(c) (unsigned char)(c & 0xff)

#define COLOR_RED (SET_COLOR(255,0,0))
#define COLOR_GREEN (SET_COLOR(0,255,0))
#define COLOR_BLUE (SET_COLOR(0,0,255))
#define COLOR_WHITE (SET_COLOR(255,255,255))
#define COLOR_BLACK (SET_COLOR(0,0,0))

#endif
