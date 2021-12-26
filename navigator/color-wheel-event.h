#ifndef _COLOR_WHEEL_EVENT_H_
#define _COLOR_WHEEL_EVENT_H_

#include "navigator.h"
#include "color-wheel.h"

void DK_ColorWheelEvent(DK_ColorWheel *whl, NV_Docker * docker, NV_Panel * panel, int x, int y);

void DK_ColorWheelHueEvent(DK_ColorWheel *whl, NV_Docker * docker, NV_Panel * panel, int x, int y);
void DK_ColorWheelHueUpdate(DK_ColorWheel *whl, NV_Docker * docker, NV_Panel * panel, double x, double y);

void DK_ColorWheelTriangleEvent(DK_ColorWheel *whl, NV_Docker * docker, NV_Panel * panel, int x, int y);
void DK_ColorWheelTriangleUpdate(DK_ColorWheel *whl, NV_Docker * docker, NV_Panel * panel, double x, double y);

#endif // _COLOR_WHEEL_EVENT_H_
