#ifndef _CORE_SYS_H_
#define _CORE_SYS_H_

#include <SDL2/SDL.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define rmask  0xff000000
#define gmask  0x00ff0000
#define bmask  0x0000ff00
#define amask  0x000000ff

#define rshift 24
#define gshift 16
#define bshift 8
#define ashift 0

#else
#define rmask  0x000000ff
#define gmask  0x0000ff00
#define bmask  0x00ff0000
#define amask  0xff000000

#define rshift 0
#define gshift 8
#define bshift 16
#define ashift 24

#endif

/*
 * add cursors
 */
Uint32 SYS_RGBA(Uint8 r,Uint8 g,Uint8 b,Uint8 a);

Uint8 SYS_GetAlphaXY(SDL_Surface * surface,int x, int y );
Uint8 SYS_GetAlpha(SDL_Surface * surface,int i );

void SYS_SetAlpha(SDL_Surface * surface,int i ,Uint8 a);

Uint8 SYS_GetRed(SDL_Surface * surface,int i );
Uint8 SYS_GetRedXY(SDL_Surface * surface,int x, int y );
void SYS_SetRed(SDL_Surface * surface,int i ,Uint8 r);

Uint8 SYS_GetGreen(SDL_Surface * surface,int i );
Uint8 SYS_GetGreenXY(SDL_Surface * surface,int x, int y );
void SYS_SetGreen(SDL_Surface * surface,int i ,Uint8 g);

Uint8 SYS_GetBlue(SDL_Surface * surface,int i );
Uint8 SYS_GetBlueXY(SDL_Surface * surface,int x, int y );
void SYS_SetBlue(SDL_Surface * surface,int i ,Uint8 a);

void SYS_SetPixel( SDL_Surface *surface, int i, Uint32 pixel );
void MZSDL_PutPixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );

int SYS_InsideRect(int x,int y, SDL_Rect * rect);

void SYS_DrawVerticalLine(SDL_Surface * surface,int x,int y0,int y1,Uint32 color);
void SYS_DrawHorizantalLine(SDL_Surface * surface,int x0,int x1,int y,Uint32 color);

void SYS_DrawOpactiy_0(SDL_Surface * surface,SDL_Rect * dest,int size);

#endif // _CORE_SYS_H_
