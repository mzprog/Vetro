#ifndef _NAVIGATOR_EVENT_H_
#define _NAVIGATOR_EVENT_H_

#include "navigator.h"

void NV_NavigatorMouseEvent(int x, int y);
void NV_PanelMouseEvent(int x,int y,NV_Panel * panel);

void NV_HeadPanelMouseEvent(int x,int y,NV_Panel * panel);

void NV_DockerMouseEvent(int x,int y,NV_Panel * panel);

void NV_SwicthDocker(NV_Panel * panel, NV_Docker * docker);

#endif // _NAVIGATOR_EVENT_H_
