#include "drawable.h"

#include <SDL.h>
#include <stdlib.h>

#include "file.h"
#include "selection.h"
#include "../core/sys.h"


#include <stdio.h>



unsigned char * drawable=NULL;//the area of the alpha channel of the layer
unsigned int  drawablesize=0;

int drawablex;
int drawabley;

SDL_Surface * drawableBuffer=NULL;


int DR_Initdrawable()
{
    int i,length;
    length=mainFile->w * mainFile->h;

    drawablex=mainFile->w;
    drawabley=mainFile->h;

    drawable=(unsigned char * ) malloc(length);
    if(drawable==NULL)
    {
        return 0;
    }

    for(i=0;i<length;i++)
    {
        drawable[i]=0xff;
    }
    drawablesize=length;
    return 1;
}

SDL_Surface  * DR_GetDrawableBuffer()
{
    return drawableBuffer;
}
void DR_SetDrawableBuffer(SDL_Surface * surface)
{
    drawableBuffer=surface;
}
unsigned char * DR_GetDrawable()
{
    return drawable;
}

unsigned int DR_GetDrawableSize()
{
    return drawablesize;
}

void DR_FillDrawable(Uint8 value)
{
    int i;
    for(i=0;i<drawablesize;i++)
        drawable[i]=value;
}

void DR_SetDrawbleXY(int x,int y,int value)
{
    if(x>=0 && x<drawablex && y>=0 && y<drawabley)
        drawable[( y * drawablex ) + x]=value;
}
unsigned char DR_GetDrawableXY(int x, int y)
{
    if(x>=0 && x<drawablex && y>=0 && y<drawabley)
    {
        return drawable[( y * drawablex ) + x];
    }
    else
    {
        return 0;
    }
}

void DR_SetBufferToDrawableData(SDL_Surface * surface)
{
    int i, a;
    int length=surface->w * surface->h;
    if(DR_GetSelectMode())
    {
        for(i=0;i<length;i++)
        {
            a=(SYS_GetAlpha(surface,i) * drawable[i])/0xff;
            SYS_SetAlpha(surface,i , a);
        }
    }
}

void DR_BlitDrawable()
{
    int i,length;
    Uint8 a, asrc,adst;//alpha channel
    Uint8 r,g,b;
    double dA,dS,dD;

    length=mainFile->w*mainFile->h;

    DR_SetBufferToDrawableData(drawableBuffer);
    for(i=0;i<length;i++)
    {
        asrc=SYS_GetAlpha(drawableBuffer,i);
        if(asrc==0)
        {
            continue;
        }

        if(asrc!=0xff)
        {

            adst=SYS_GetAlpha(mainFile->activeLayer->surface,i);
            a=asrc + adst*(255-asrc)/255;

            dA=a/255.0;
            dS=asrc/255.0;
            dD=adst/255.0;

            r=(SYS_GetRed(drawableBuffer,i)*dS      +   SYS_GetRed(mainFile->activeLayer->surface,i)  * dD *(1-dS))/dA;//check more
            g=(SYS_GetGreen(drawableBuffer,i)*dS    +   SYS_GetGreen(mainFile->activeLayer->surface,i)* dD *(1-dS))/dA;
            b=(SYS_GetBlue(drawableBuffer,i)*dS     +   SYS_GetBlue(mainFile->activeLayer->surface,i) * dD *(1-dS))/dA;

        }
        else
        {
            a=asrc;
            r=SYS_GetRed(drawableBuffer,i);
            g=SYS_GetGreen(drawableBuffer,i);
            b=SYS_GetBlue(drawableBuffer,i);
        }
        SYS_SetPixel(mainFile->activeLayer->surface,i,SYS_RGBA(r,g,b,a));
    }
    SDL_FillRect(drawableBuffer,NULL,0);
    DF_UpdateFile();
}

