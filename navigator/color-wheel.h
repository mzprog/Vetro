#ifndef _COLOR_WHEEL_H_
#define _COLOR_WHEEL_H_

#include <SDL2/SDL.h>

typedef struct{
    SDL_Rect rect;
    SDL_Surface * surface;
    SDL_Surface * colors;

    //int minR;
    //int maxR;

    int p1X;
    int p1Y;
    int p2X;
    int p2Y;
    int p3X;
    int p3Y;

} DK_ColorWheel;

DK_ColorWheel * DK_DrawColorWheel(SDL_Rect * rect);
void DK_DrawWheelCircle(SDL_Surface * surface, int r, int X,int Y);
void DK_DrawWheelCircle_Outline(SDL_Surface * surface, int r, int X,int Y);
void DK_DrawColorTriangle(DK_ColorWheel * whl);
void DK_DrawWheelMarker(DK_ColorWheel * whl);

void DK_UpdateWheel(DK_ColorWheel * whl);



#endif // _COLOR_WHEEL_H_
