#ifndef _LINE_PLOT_H_
#define _LINE_PLOT_H_

#include <SDL2/SDL.h>

void LINE_PlotPoint(SDL_Surface * surface,int x,int y, double alpha);

int LINE_IPart(double d);
int LINE_Round(double d);

double LINE_FPart(double d);
double LINE_RFPart(double d);

void LINE_Swap(double * a, double * b);

#endif // _LINE_PLOT_H_
