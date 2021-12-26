#ifndef _ZOOM_H_
#define _ZOOM_H_

#include "tools.h"

#define ZOOMIN  0
#define ZOOMOUT 1

int TOOL_SetZoom(DR_Tools * zoom);
void TOOL_UnSetZoom(DR_Tools * zoom);

void TOOL_SetValueZoom( int value);
void TOOL_UseZoom(int x,int y);

#endif // _ZOOM_H
