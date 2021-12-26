#include "line-plot.h"

#include <SDL.h>

#include "../core/sys.h"
#include "../core/color.h"
#include "../file/file.h"

void LINE_PlotPoint(SDL_Surface * surface,int x,int y, double alpha)
{
    if(x<0 || y<0 || x > mainFile->w-1 || y > mainFile->h-1)
    {
        return;
    }

   // Uint32 *pixels = (Uint32 *)surface->pixels;
	Uint32 color=SYS_GetForegroundColor();


	//Uint8 a= alpha*255;
	Uint8 a, asrc;//,adst;//alpha channel
    //Uint8 r,g,b;


    if(alpha>1)
    {
        alpha=1;
    }
    asrc=(color &amask)>>ashift;
    asrc*=alpha;
    //adst=SYS_GetAlphaXY(surface,x,y);
/*
    if(asrc!=0xff)
    {
        a= asrc + adst*(255-asrc)/255;
printf("src=%d\tdst=%d\n",asrc,adst);
        r=(((color &rmask) >>rshift)*asrc/255 +(SYS_GetRedXY(surface,x,y)*adst/255 *(255-asrc))/255);
        g=((color &gmask >>gshift)*asrc/255 +(SYS_GetGreenXY(surface,x,y)*adst/255 *(255-asrc))/255);
        b=((color &bmask >>bshift)*asrc/255 +(SYS_GetBlueXY(surface,x,y)*adst/255 *(255-asrc))/255);


        color=SYS_RGBA(r,g,b,a);
printf("r=%d\tg=%d\tb=%d\ta=%d\n",r,g,b,a);
   }
    else
    {*/
        a=asrc;
        color= color & ~amask;
        color= color | (a<<ashift);
 //   }

    MZSDL_PutPixel32(surface,x,y,color);
	//pixel&=~amask;
	//pixel |= a<<ashift;



	//pixels[ ( y * surface->w ) + x ] =pixel ;
}

int LINE_IPart(double d)
{
    return (int)d;
}

int LINE_Round(double d)
{
    return LINE_IPart(d + 0.5);
}

double LINE_FPart(double d)
{
    return d - LINE_IPart(d);
}

double LINE_RFPart(double d)
{
    return 1- LINE_FPart(d);
}

void LINE_Swap(double * a, double * b)
{
    double t=*a;
    *a=*b;
    *b=t;
}
