#ifndef _GUI_TOOLS_H_
#define _GUI_TOOLS_H_

#include <SDL2/SDL.h>

void KIT_DrawBlendedLine(SDL_Surface * surface, double x0,double y0,double x1, double y1, Uint32 color);
void KIT_PlotLinePoint( SDL_Surface * surface,int x,int y,double alpha,Uint32 color);

#endif // _GUI_TOOLS_H_
