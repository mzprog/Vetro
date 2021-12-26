#ifndef _LAYERS_DOCK_H_
#define _LAYERS_DOCK_H_

#include "navigator.h"

int DK_CreateLayerDocker(NV_Docker * docker);
void DK_OpacitySetValue(int x);

void DK_DestroyLayerDocker(struct NV_Docker *docker);
void DK_UseLayerDocker(NV_Docker * docker,NV_Panel * panel,int x,int y);
void DK_UpdateLayerDock();



#endif // _LAYERS_DOCK_H_
