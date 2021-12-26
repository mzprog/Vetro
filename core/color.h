#ifndef _COLOR_H_
#define _COLOR_H_

#include <SDL2/SDL.h>

#define HUE_UPPER_LIMIT 360

typedef struct
{
    double H;
    double S;
    double L;
} HslColor;



Uint32 SYS_GetForegroundColor();

Uint8 SYS_GetForegroundRedColor();
Uint8 SYS_GetForegroundGreenColor();
Uint8 SYS_GetForegroundBlueColor();

double SYS_GetForegroundHueColor();
double SYS_GetForegroundSaturationColor();
double SYS_GetForegroundBrightnessColor();

void SYS_SetForegroundHSLColor(double h,double s,double l);

void SYS_SetForegroundRedColor(Uint8 r);
void SYS_SetForegroundGreenColor(Uint8 g);
void SYS_SetForegroundBlueColor(Uint8 b);

void COLOR_RGB_TO_HSL(double r, double g, double b ,HslColor* color);
void COLOR_HSL_To_RGB(double H, double S, double V);
Uint32 SYS_GetRGBAFromColor();

void COLOR_UpdateHSL();
void COLOR_UpdateRGB();

#endif // _COLOR_H_
