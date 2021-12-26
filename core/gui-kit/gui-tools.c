#include "gui-tools.h"

#include <SDL.h>
#include <stdlib.h>

#include "../sys.h"
#include "../../tools/line-plot.h"

void KIT_DrawBlendedLine(SDL_Surface * surface, double x0,double y0,double x1, double y1, Uint32 color)
{

    double dx,dy;
    double gradient,gradabs,alpha;
    double xend, yend, xpxl1, ypxl1, xpxl2, ypxl2;
    double xgap,intery;
    int x,inetY;

    char steep =abs(y1 - y0) > abs(x1 - x0);
    if(steep)
    {
        LINE_Swap(&x0,&y0);
        LINE_Swap(&x1,&y1);
    }
    if(x0>x1)
    {
        LINE_Swap(&x0,&x1);
        LINE_Swap(&y0,&y1);
    }

    dx = x1 - x0;
    dy = y1 - y0;
    gradient = dy / dx;

    if(dx==0.0)
    {
        gradient=1;
    }
    gradabs=gradient>=0?gradient:-1*gradient;
//printf("gradient= %g\n",gradient);
//printf("abs= %g\n",gradabs);
    // handle first endpoint
    xend = LINE_Round(x0);
    yend = y0 + gradient * (xend - x0);
    xgap = LINE_RFPart(x0 + 0.5);
    xpxl1 = xend; // this will be used in the main loop
    ypxl1 = LINE_IPart(yend);

    if(steep)
    {
        if(gradient<0)
            KIT_PlotLinePoint(surface, ypxl1-1,   xpxl1, LINE_FPart(yend) * xgap+gradabs/2,color);
        KIT_PlotLinePoint(surface, ypxl1,   xpxl1, LINE_RFPart(yend) * xgap,color);
        if(gradient>0)
            KIT_PlotLinePoint(surface, ypxl1+1,   xpxl1, LINE_FPart(yend) * xgap+gradabs/2,color);
//puts("steep");
    }
    else
    {
        if(gradient<0)
            KIT_PlotLinePoint(surface, xpxl1,   ypxl1-1, LINE_FPart(yend) * xgap+gradabs/2,color);
        KIT_PlotLinePoint(surface, xpxl1,   ypxl1, LINE_RFPart(yend) * xgap,color);
        if(gradient>0)
            KIT_PlotLinePoint(surface, xpxl1,   ypxl1+1, LINE_FPart(yend) * xgap+gradabs/2,color);
    }

    intery = yend + gradient; // first y-intersection for the main loop
    inetY=intery;

    // handle second endpoint
    xend = LINE_Round(x1);
    yend = y1 + gradient * (xend - x1);
    xgap = LINE_FPart(x1 + 0.5);
    xpxl2 = xend; //this will be used in the main loop
    ypxl2 = LINE_IPart(yend);

    if(steep)
    {
        if(gradient<0)
            KIT_PlotLinePoint(surface, ypxl2-1,   xpxl2, LINE_FPart(yend) * xgap +gradabs/2,color);
        KIT_PlotLinePoint(surface, ypxl2,   xpxl2, LINE_RFPart(yend) * xgap,color);
        if(gradient>0)
            KIT_PlotLinePoint(surface, ypxl2+1,   xpxl2, LINE_FPart(yend) * xgap +gradabs/2,color);
    }
    else
    {
        if(gradient>0)
            KIT_PlotLinePoint(surface, xpxl2,   ypxl2-1, LINE_FPart(yend) * xgap +gradabs/2,color);
        KIT_PlotLinePoint(surface, xpxl2,   ypxl2, LINE_RFPart(yend) * xgap, color);
        if(gradient<0)
            KIT_PlotLinePoint(surface, xpxl2,   ypxl2+1, LINE_FPart(yend) * xgap +gradabs/2,color);
    }

    // main loop
    if(steep)
    {
 //printf("color %d\n",(color&rmask) >>rshift);
        alpha=gradabs/2;
       for(x=xpxl1+1;x<xpxl2;x++)
       {

            //if(alpha>1) alpha=gradabs/2;
            if(gradient>0)
            {
                KIT_PlotLinePoint(surface, LINE_IPart(intery), x,  1-alpha, color);//maybe adding 0.5 * rfpart
                //KIT_PlotLinePoint(surface, LINE_IPart(intery)  , x, 1, color);
                KIT_PlotLinePoint(surface, LINE_IPart(intery)+1, x,  alpha, color);
                intery = intery + gradient;
                alpha+=gradabs;
               if(LINE_IPart(intery)> inetY) alpha=gradabs/2;
               //if(alpha>1) alpha=1 -alpha;
            }
            else
            {
                KIT_PlotLinePoint(surface, LINE_IPart(intery), x,  alpha, color);//maybe adding 0.5 * rfpart
                //KIT_PlotLinePoint(surface, LINE_IPart(intery)  , x, 1, color);
                KIT_PlotLinePoint(surface, LINE_IPart(intery)+1, x,  1-alpha, color);
                intery = intery + gradient;
                alpha+=gradabs;
               if(LINE_IPart(intery)< inetY) alpha=gradabs/2;
            }

            inetY=intery;
       }
    }
    else
    {

        for(x=xpxl1+1;x<xpxl2;x++)
       {
            KIT_PlotLinePoint(surface, x,  LINE_IPart(intery)-1, LINE_FPart(intery)+gradabs/2, color);
            KIT_PlotLinePoint(surface, x,  LINE_IPart(intery)  , LINE_RFPart(intery)+gradabs/2, color);
            KIT_PlotLinePoint(surface, x,  LINE_IPart(intery)+1, LINE_FPart(intery)+gradabs/2, color);
            intery = intery + gradient;
       }
    }
}

void KIT_PlotLinePoint( SDL_Surface * surface,int x,int y,double alpha,Uint32 color)
{
    Uint8 a, asrc,adst;//alpha channel
    Uint8 r,g,b;

    if(alpha>1)
    {
        alpha=1;
    }
    asrc=(color &amask)>>ashift;
    asrc*=alpha;
    adst=SYS_GetAlphaXY(surface,x,y);

    if(asrc!=0xff)
    {
        a= asrc + adst*(255-asrc)/255;



        r=(((color &rmask) >>rshift)*asrc/255 +(SYS_GetRedXY(surface,x,y)*adst/255 *(255-asrc))/255);
        g=(((color &gmask) >>gshift)*asrc/255 +(SYS_GetGreenXY(surface,x,y)*adst/255 *(255-asrc))/255);
        b=(((color &bmask) >>bshift)*asrc/255 +(SYS_GetBlueXY(surface,x,y)*adst/255 *(255-asrc))/255);

        color=SYS_RGBA(r,g,b,a);
   }
    else
    {
        a=asrc;
        color= color & ~amask;
        color= color | (a<<ashift);
    }

    MZSDL_PutPixel32(surface,x,y,color);
}
