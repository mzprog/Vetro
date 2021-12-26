#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include <SDL2/SDL.h>
//extern SDL_Surface * drawableBuffer;
int DR_Initdrawable();

unsigned char * DR_GetDrawable();//unset
SDL_Surface * DR_GetDrawableBuffer();
void DR_SetDrawableBuffer(SDL_Surface * surface);
unsigned int DR_GetDrawableSize();

void DR_SetBufferToDrawableData(SDL_Surface * surface);

void DR_SetDrawbleXY(int x,int y,int value);
unsigned char DR_GetDrawableXY(int x, int y);

void DR_FillDrawable(Uint8 value);
void DR_BlitDrawable();

#endif // _DRAWABLE_H_
