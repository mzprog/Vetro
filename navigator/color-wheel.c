#include "color-wheel.h"

#include <SDL.h>
#include <stdlib.h>
#include <math.h>

#include "../core/sys.h"
#include "../core/color.h"
#include "../core/gui-kit/gui-tools.h"
#include "../tools/line-plot.h"

DK_ColorWheel* wheel=NULL;

DK_ColorWheel * DK_DrawColorWheel(SDL_Rect * rect)
{
    int r;
    wheel=(DK_ColorWheel *) malloc(sizeof(DK_ColorWheel));
    if(wheel==NULL)
    {
        return NULL;
    }

    if(rect->w > rect->h)
    {
        wheel->rect.w=rect->h;
        wheel->rect.h=rect->h;
        wheel->rect.y=rect->y;
        wheel->rect.x=(rect->w - rect->h)/2 + rect->x;

    }
    else
    {
        wheel->rect.w=rect->w;
        wheel->rect.h=rect->w;
        wheel->rect.x=rect->x;
        wheel->rect.y=(rect->h - rect->w)/2 +rect->y;
    }

    wheel->surface=SDL_CreateRGBSurface(0,wheel->rect.w, wheel->rect.h, 32, rmask, gmask, bmask, amask);
    if(wheel->surface==NULL)
    {
        free(wheel);
        return NULL;
    }
    wheel->colors=SDL_CreateRGBSurface(0,wheel->rect.w,wheel->rect.h,32,rmask,gmask,bmask,amask);
    if(wheel->colors==NULL)
    {
        SDL_FreeSurface(wheel->surface);
        free(wheel);
        return NULL;
    }

    SDL_FillRect(wheel->colors,NULL,SYS_RGBA(0x8f,0x8f,0x8f,0xff));
    //circle
    DK_DrawWheelCircle(wheel->colors,wheel->rect.w/2-1 ,wheel->rect.w/2,wheel->rect.h/2);

    SDL_BlitSurface(wheel->colors,NULL,wheel->surface,NULL);
    //triangle
    r=wheel->rect.w/2-25;
    wheel->p1X = r * sin(0) + wheel->rect.w/2;
    wheel->p1Y =-r * cos(0) + wheel->rect.w/2;
    wheel->p2X = r * sin(M_PI*120/180) + wheel->rect.w/2;
    wheel->p2Y =-r * cos(M_PI*120/180) + wheel->rect.w/2;
    wheel->p3X = r * sin(M_PI*240/180) + wheel->rect.w/2;
    wheel->p3Y =-r * cos(M_PI*240/180) + wheel->rect.w/2;

    DK_DrawColorTriangle(wheel);

    //draw markers
    DK_DrawWheelMarker(wheel);

    return wheel;
}

void DK_DrawWheelCircle(SDL_Surface * surface, int r, int X,int Y)
{
    int i,j,rs,rr;
    double i2;
    double hue;
    Uint32 color;

    rs=r-20;

    DK_DrawWheelCircle_Outline(surface,r,X,Y);
    DK_DrawWheelCircle_Outline(surface,rs,X,Y);

    for(i=0;i<surface->w/2;i++)
    {
        i2=i*i;
        for(j=0;j<surface->h/2;j++)
        {
            rr=sqrt(i2 + j*j);
            if(rr<r && rr>=rs)
            {
                hue= (180/M_PI)*acos(i/sqrt(i2+j*j));

                COLOR_HSL_To_RGB(90-hue,100,100);
                color=SYS_GetRGBAFromColor();
                KIT_PlotLinePoint(surface,i+X,Y-j-1,1,color);

                COLOR_HSL_To_RGB(90+hue,100,100);
                color=SYS_GetRGBAFromColor();
                KIT_PlotLinePoint(surface,i+X,j+Y,1,color);

                COLOR_HSL_To_RGB(270-hue,100,100);
                color=SYS_GetRGBAFromColor();
                KIT_PlotLinePoint(surface,X-i-1,j+Y,1,color);

                COLOR_HSL_To_RGB(270+hue,100,100);
                color=SYS_GetRGBAFromColor();
                KIT_PlotLinePoint(surface,X-i-1,Y-j-1,1,color);
            }
        }
    }
}

void DK_DrawWheelCircle_Outline(SDL_Surface * surface, int r, int X,int Y)
{
    double x,y;
    double r2=r*r;
    double alpha,alpha2;
    double ralpha,ralpha2;
    double hue;
    Uint32 color;
    int w=r/sqrt(2) ;

    for(x=0;x<w;x+=y/r)
    {
        y=sqrt(r2-x*x);
        alpha=LINE_FPart(y)==0?1:LINE_FPart(y);
        ralpha=1-alpha;

        alpha2=LINE_FPart(y);
        ralpha2=1-alpha2;
        hue= (180/M_PI)*acos(x/r);
        hue -=45;

        COLOR_HSL_To_RGB(45-hue,100,100);
        color=SYS_GetRGBAFromColor();
        KIT_PlotLinePoint(surface,X+x,Y-y-1,alpha,color);//0-45
        KIT_PlotLinePoint(surface,X+x,Y-y,ralpha,color);

        COLOR_HSL_To_RGB(45+hue,100,100);
        color=SYS_GetRGBAFromColor();
        KIT_PlotLinePoint(surface,Y+y,X-x,ralpha2,color);//45-90
        KIT_PlotLinePoint(surface,Y+y+1,X-x,alpha2,color);

        COLOR_HSL_To_RGB(135-hue,100,100);//
        color=SYS_GetRGBAFromColor();
        KIT_PlotLinePoint(surface,Y+y,X+x,ralpha2,color);//90-135
        KIT_PlotLinePoint(surface,Y+y+1,X+x,alpha2,color);

        COLOR_HSL_To_RGB(135+hue,100,100);//
        color=SYS_GetRGBAFromColor();
        KIT_PlotLinePoint(surface,X+x,Y+y,ralpha2,color);//135-180
        KIT_PlotLinePoint(surface,X+x,Y+y+1,alpha2,color);


        COLOR_HSL_To_RGB(225-hue,100,100);//
        color=SYS_GetRGBAFromColor();
        KIT_PlotLinePoint(surface,X-x,Y+y,ralpha2,color);//180-225
        KIT_PlotLinePoint(surface,X-x,Y+y+1,alpha2,color);

        COLOR_HSL_To_RGB(225+hue,100,100);//
        color=SYS_GetRGBAFromColor();
        KIT_PlotLinePoint(surface,Y-y-1,X+x,alpha,color);//225-270
        KIT_PlotLinePoint(surface,Y-y,X+x,ralpha,color);

        COLOR_HSL_To_RGB(315-hue,100,100);//
        color=SYS_GetRGBAFromColor();
        KIT_PlotLinePoint(surface,Y-y-1,X-x,alpha,color);//270-315
        KIT_PlotLinePoint(surface,Y-y,X-x,ralpha,color);

        COLOR_HSL_To_RGB(315+hue,100,100);//
        color=SYS_GetRGBAFromColor();
        KIT_PlotLinePoint(surface,X-x,Y-y-1,alpha,color);//315-360
        KIT_PlotLinePoint(surface,X-x,Y-y,ralpha,color);
    }

    y=sqrt(r2-w*w);
    alpha=LINE_FPart(y)==0?1:LINE_FPart(y);
    ralpha=1-alpha;

    alpha2=LINE_FPart(y);
    ralpha2=1-alpha2;

    hue= (180/M_PI)*acos(x/r);
    hue -=45;

    COLOR_HSL_To_RGB(45-hue,100,100);
    color=SYS_GetRGBAFromColor();
    KIT_PlotLinePoint(surface,X+x,Y-y-1,alpha,color);//0-45
    KIT_PlotLinePoint(surface,X+x,Y-y,ralpha,color);

    COLOR_HSL_To_RGB(45+hue,100,100);
    color=SYS_GetRGBAFromColor();
    KIT_PlotLinePoint(surface,Y+y,X-x,ralpha2,color);//45-90
    KIT_PlotLinePoint(surface,Y+y+1,X-x,alpha2,color);

    COLOR_HSL_To_RGB(135-hue,100,100);//
    color=SYS_GetRGBAFromColor();
    KIT_PlotLinePoint(surface,Y+y,X+x,ralpha2,color);//90-135
    KIT_PlotLinePoint(surface,Y+y+1,X+x,alpha2,color);

    COLOR_HSL_To_RGB(135+hue,100,100);//
    color=SYS_GetRGBAFromColor();
    KIT_PlotLinePoint(surface,X+x,Y+y,ralpha2,color);//135-180
    KIT_PlotLinePoint(surface,X+x,Y+y+1,alpha2,color);

    COLOR_HSL_To_RGB(225-hue,100,100);//
    color=SYS_GetRGBAFromColor();
    KIT_PlotLinePoint(surface,X-x,Y+y,ralpha2,color);//180-225
    KIT_PlotLinePoint(surface,X-x,Y+y+1,alpha2,color);

    COLOR_HSL_To_RGB(225+hue,100,100);//
    color=SYS_GetRGBAFromColor();
    KIT_PlotLinePoint(surface,Y-y-1,X+x,alpha,color);//225-270
    KIT_PlotLinePoint(surface,Y-y,X+x,ralpha,color);

    COLOR_HSL_To_RGB(315-hue,100,100);//
    color=SYS_GetRGBAFromColor();
    KIT_PlotLinePoint(surface,Y-y-1,X-x,alpha,color);//270-315
    KIT_PlotLinePoint(surface,Y-y,X-x,ralpha,color);

    COLOR_HSL_To_RGB(315+hue,100,100);//
    color=SYS_GetRGBAFromColor();
    KIT_PlotLinePoint(surface,X-x,Y-y-1,alpha,color);//315-360
    KIT_PlotLinePoint(surface,X-x,Y-y,ralpha,color);
}

void DK_DrawColorTriangle(DK_ColorWheel * whl)
{
    int y0,y1,yDiff;
    int x0,x1,xDiff;
    double a13,a12;
    double c13,c12;
    double Hue,Brightness,Saturation;
    Uint32 color;

    int i,j;//for loops
    //start and end height
    y0 = whl->p1Y;
    y1 = whl->p2Y;
    yDiff = y1 - y0;
    //line slopes
    a13 = ((whl->p1Y - whl->p3Y) * 1.0) / (whl->p1X - whl->p3X);
    a12 = ((whl->p1Y - whl->p2Y) * 1.0) / (whl->p1X - whl->p2X);
    //calculate c
    c13 = whl->p1Y - a13 * whl->p1X;
    c12 = whl->p1Y - a12 * whl->p1X;
    //get hue
//printf("1h=%g\n",SYS_GetForegroundHueColor());
    //COLOR_UpdateHSL();
    //printf("2h=%g\n",SYS_GetForegroundHueColor());
    Hue = SYS_GetForegroundHueColor();

    for(j=y0;j<=y1;j++)
    {
        x0 = (j - c13) / a13;
        x1 = (j - c12) / a12;
        xDiff = x1 - x0;
        if(xDiff==0)xDiff=1;
        //Brightness
        Brightness = (j - y0) * (100.0 /yDiff);
        for(i=x0;i<=x1;i++)
        {
            Saturation = ((double)i-x0)/xDiff * 100;
            COLOR_HSL_To_RGB(Hue,Saturation,Brightness);//
            color=SYS_GetRGBAFromColor();
            KIT_PlotLinePoint(whl->surface,i,j,1,color);
        }
    }
}

void DK_DrawWheelMarker(DK_ColorWheel * whl)
{
    //int i,j;
    int x,y;
    int x0,x1,y1,xDiff;
    int centerX,centerY;
    double Hue,Brightness,Saturation;
    double hueShift;
    double a13,a12;
    double c13,c12;

    Hue = SYS_GetForegroundHueColor();
    Brightness = SYS_GetForegroundBrightnessColor();
    Saturation = SYS_GetForegroundSaturationColor();

    hueShift= 360 - Hue + 90;
    if(hueShift>360) hueShift -=360;


    centerX=wheel->rect.w/2;
    centerY=wheel->rect.h/2;

    x=centerX * cos(hueShift * M_PI/180) + centerX;
    y=-centerY * sin(hueShift * M_PI/180) + centerY;
    x1=(centerX - 20) * cos(hueShift * M_PI/180) + centerX;
    y1=-(centerY - 20) * sin(hueShift * M_PI/180) + centerY;

/*
    if(Hue <= 90 || Hue >= 270)
    {
        y  = centerY - y;
        y1 = centerY - y1;
    }
    else
    {
        y  = centerY + y;
        y1 = centerY + y1;
    }
    if(Hue <= 180)
    {
        x  = centerX + x;
        x1 = centerX + x1;
    }
    else
    {
        x  = centerX - x;
        x1 = centerX - x1;
    }
*/
    KIT_DrawBlendedLine(whl->surface,x,y,x1,y1,SYS_RGBA(0xff, 0xff, 0xff, 0xff));

    //triangle
    y = whl->p1Y + (whl->p2Y - whl->p1Y)* (Brightness/100);
     //line slopes
    a13 = ((whl->p1Y - whl->p3Y) * 1.0) / (whl->p1X - whl->p3X);
    a12 = ((whl->p1Y - whl->p2Y) * 1.0) / (whl->p1X - whl->p2X);

    //calculate c
    c13 = whl->p1Y - a13 * whl->p1X;
    c12 = whl->p1Y - a12 * whl->p1X;
    //calculate line width
    x0 = (y - c13) / a13;
    x1 = (y - c12) / a12;
    xDiff = x1 - x0;
    //postion of x
    x= x0 + xDiff * Saturation/100;

    MZSDL_PutPixel32(whl->surface,x,y,SYS_RGBA(0xff,0xff,0xff,0xff));
    MZSDL_PutPixel32(whl->surface,x+1,y,SYS_RGBA(0xff,0xff,0xff,0xff));
    MZSDL_PutPixel32(whl->surface,x-1,y,SYS_RGBA(0xff,0xff,0xff,0xff));
    MZSDL_PutPixel32(whl->surface,x,y+1,SYS_RGBA(0xff,0xff,0xff,0xff));
    MZSDL_PutPixel32(whl->surface,x,y-1,SYS_RGBA(0xff,0xff,0xff,0xff));


    MZSDL_PutPixel32(whl->surface,x-1,y-1,SYS_RGBA(0,0,0,0xff));
    MZSDL_PutPixel32(whl->surface,x+1,y-1,SYS_RGBA(0,0,0,0xff));
    MZSDL_PutPixel32(whl->surface,x-1,y+1,SYS_RGBA(0,0,0,0xff));
    MZSDL_PutPixel32(whl->surface,x+1,y+1,SYS_RGBA(0,0,0,0xff));

    MZSDL_PutPixel32(whl->surface,x+2,y,SYS_RGBA(0,0,0,0xff));
    MZSDL_PutPixel32(whl->surface,x-2,y,SYS_RGBA(0,0,0,0xff));
    MZSDL_PutPixel32(whl->surface,x,y+2,SYS_RGBA(0,0,0,0xff));
    MZSDL_PutPixel32(whl->surface,x,y-2,SYS_RGBA(0,0,0,0xff));

}

void DK_UpdateWheel(DK_ColorWheel * whl)
{
    SDL_BlitSurface(whl->colors,NULL,whl->surface,NULL);
    DK_DrawColorTriangle(whl);
    DK_DrawWheelMarker(whl);

}

