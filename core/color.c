#include "color.h"

#include <stdlib.h>
#include <math.h>

#include "sys.h"

Uint32 foregroundColor= amask;//black color by default
int r,g,b;
double H=0,S=0,L=0;

Uint32 SYS_GetForegroundColor()
{
    return foregroundColor;
}

Uint8 SYS_GetForegroundRedColor()
{
    return (foregroundColor & rmask) >> rshift;
}
Uint8 SYS_GetForegroundGreenColor()
{
    return (foregroundColor & gmask) >> gshift;
}

Uint8 SYS_GetForegroundBlueColor()
{
    return (foregroundColor & bmask) >> bshift;
}

double SYS_GetForegroundHueColor()
{
    return H;
}
double SYS_GetForegroundSaturationColor()
{
    return S;
}
double SYS_GetForegroundBrightnessColor()
{
    return L;
}

void SYS_SetForegroundHSLColor(double h,double s,double l)
{
    H=h;
    S=s;
    L=l;
}

void SYS_SetForegroundRedColor(Uint8 r)
{
    foregroundColor &= ~rmask;
    foregroundColor |= (r<<rshift);
}

void SYS_SetForegroundGreenColor(Uint8 g)
{
    foregroundColor &= ~gmask;
    foregroundColor |= (g<<gshift);
}
void SYS_SetForegroundBlueColor(Uint8 b)
{
    foregroundColor &= ~bmask;
    foregroundColor |= (b<<bshift);
}

Uint32 SYS_GetRGBAFromColor()
{
    return SYS_RGBA(r,g,b,255);
}



double Double_GetMaximum(double r,double g,double b)
{
    if(r>g)
    {
        if(r>b)
        {
            return r;
        }
        else
        {
            return b;
        }
    }
    else if(g>b)
    {
        return g;
    }
    else
    {
        return b;
    }
}

double Double_GetMinimum(double r,double g,double b)
{
    if(r<g)
    {
        if(r<b)
        {
            return r;
        }
        else
        {
            return b;
        }
    }
    else if(g<b)
    {
        return g;
    }
    else
    {
        return b;
    }
}


void COLOR_RGB_TO_HSL(double r, double g, double b ,HslColor* color)
{
    double M = 0.0, m = 0.0, c = 0.0;


        M = Double_GetMaximum(r, g, b);
        m = Double_GetMinimum(r, g, b);
        c = M - m;

        //color->H=0;

        color->L=M;

        if(M!=0)
        {
            color->S=255*c/M;
        }
        else
        {
            color->S=0;
        }
        if(color->S==0)
        {
            color->L *=100.0/255;
            color->H=SYS_GetForegroundHueColor();
            return;
        }

        if(color->S!=0)
        {
            if (M == r)
            {

                color->H = fmod(((g - b) / c), 6.0);
            }
            else if (M == g)
            {
                color->H = ((b - r) / c) + 2.0;
            }
            else/*if(M==b)*/
            {
                color->H = ((r - g) / c) + 4.0;
            }
        }
        else
        {
            color->H=0;
        }

        color->H=color->H*60;
        if(color->H<0)
            color->H=color->H+360;

        color->S=color->S*100/255;
        color->L=color->L*100/255;

}

void RgbF_Create(double nr, double ng, double nb)
{
    r=nr;
    g=ng;
    b=nb;
}

void COLOR_HSL_To_RGB(double H, double S, double V)
{
    double max,delta;
    double h=H/60;
    double s=S*255/100;
    double b1=V*255/100;

    max=b1;
    if(h>5)
    {
        h=h-6;
    }
    if(s==0)
    {
        r=b1;
        g=b1;
        b=b1;
    }
    else
    {
        delta=s*max/255;
        if(h>3)
        {
            b=max;
            if(h>4)
            {
                g=max-delta;
                r=(h-4)*delta + g;
            }
            else
            {
               r=max-delta;
               g= r-(h-4)*delta;
            }
        }
        else
        {
            if(h>1)
            {
                g=max;
                if(h>2)
                {
                    r=max-delta;
                    b=(h-2)*delta+r;
                }
                else
                {
                    b=max-delta;
                    r=b-(h-2)*delta;
                }
            }
            else
            {
                if(h>-1)
                {
                    r=max;
                    if(h>0)
                    {
                        b=max-delta;
                        g=h*delta + b;
                    }
                    else
                    {
                        g=max-delta;
                        b=g-(h*delta);
                    }
                }
            }
        }
    }
}

void COLOR_UpdateHSL()
{
    HslColor c;

    COLOR_RGB_TO_HSL(SYS_GetForegroundRedColor(),
                     SYS_GetForegroundGreenColor(),
                     SYS_GetForegroundBlueColor(),
                     &c);

    H=c.H;
    S=c.S;
    L=c.L;
}

void COLOR_UpdateRGB()
{

    COLOR_HSL_To_RGB(H,S,L);

    SYS_SetForegroundRedColor(r);
    SYS_SetForegroundGreenColor(g);
    SYS_SetForegroundBlueColor(b);
}
