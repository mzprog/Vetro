#ifndef _LINE_H_
#define _LINE_H_

#include "tools.h"

int TOOL_SetLine(DR_Tools * line);
void TOOL_UnSetLine(DR_Tools * line);

void TOOL_UseLine(int x,int y);

void TOOL_DrawLine(SDL_Surface * surface,double x0,double y0,double x1,double y1,int size);

void TOOL_SetValueSize(int value);
void TOOL_BoxSetValueSize( int value);

#endif // _LINE_H_
