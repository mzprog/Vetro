#ifndef _BRUSH_H_
#define _BRUSH_H_

#include <SDL2/SDL.h>
#include "tools.h"


typedef struct TOOL_BrushPoint
{
    int x;
    int y;
    int alpha;
    int size;
    struct TOOL_BrushPoint * next;
} TOOL_BrushPoint;

int TOOL_SetBrush(DR_Tools * brush);

void TOOL_DrawCircle(SDL_Surface * surface, int raduis, int centerX, int centerY);

void TOOL_DrawBrush(SDL_Surface *surface,int x, int y);
void TOOL_FillLine(SDL_Surface * surface,int x0,int y,int x1);

void TOOL_UseBrush(int x,int y);

int TOOL_ThreadBrushUpdate(void * data);
int TOOL_ADDBrushPoint(int x,int y,int bsize,int alpha);
int TOOL_PointsDrawThread(void * data);


void TOOL_UnSetBrush(DR_Tools * brush);

void TOOL_UpdateBrushSurface(Uint32 color);

void TOOL_BrushSetValueSize(int value);
void TOOL_BrushBoxSetValueSpace(int value);
void TOOL_BrushBoxSetValueSize( int value);
void TOOL_BrushSetValueSpace( int value);

void TOOL_SetValueMode(int value);

#endif // _BRUSH_H_
