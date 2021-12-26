#ifndef _COLOR_PICKER_H_
#define _COLOR_PICKER_H_

#include "navigator.h"



int DK_CreateColorPicker(NV_Docker * d);

void DK_SetRedValue(int x);
void DK_SetGreenValue(int x);
void DK_SetBlueValue(int x);

void DK_UpdateRGBScrollBox();

void DK_UnsetColorPicker(NV_Docker * d);

void DK_UseColorPicker(NV_Docker * docker, NV_Panel * panel ,int x,int y);


#endif // _COLOR_PICKER_H_
