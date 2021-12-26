#ifndef _RECTSELECT_H_
#define _RECTSELECT_H_

#include "tools.h"

int TOOL_SetRectSelect(DR_Tools * select);
void TOOL_UnsSetRectSelect(DR_Tools * select);

void TOOL_RectSelect(int x,int y);

void TOOL_DrawSelectionGuide(SDL_Surface * surface,int x,int y,int i,int j);
int TOOL_DrawSelectionRect(SDL_Surface *surface,int x,int y,int i,int j);

#endif // _RECTSELECT_H_
