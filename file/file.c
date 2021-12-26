#include "file.h"

#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "../DR-viewport.h"
#include "../core/sys.h"
#include "../mzsdl.h"
#include "layer.h"

DR_MainViewport * DrawPort = NULL;
DF_File * mainFile=NULL;

int DF_CreateNewFile(int w, int h)
{
    mainFile=(DF_File *) malloc(sizeof(DF_File));
    if(mainFile==NULL)
    {
        return 0;
    }
    mainFile->w=w;
    mainFile->h=h;
    mainFile->zoom=100;
    mainFile->layerCount=0;
    strcpy(mainFile->name,"Untitled");

    mainFile->surface=SDL_CreateRGBSurface(0,w,h,32,rmask,gmask,bmask,amask);
    if(mainFile->surface==NULL)
    {
        return 0;
    }

    mainFile->layer=NULL;
    if(DF_CreateLayer(mainFile,0xffffffff)==0)
    {
        return 0;
    }

    if(DF_CreateLayer(mainFile,0)==0)
    {
        return 0;
    }


    if(!DrawPort)
    {
        DrawPort=DR_FindViewport("drawp");
    }
    if(!DrawPort)
    {
        return 0;
    }

    mainFile->rect.w=w;
    mainFile->rect.h=h;

    mainFile->rect.x= (DrawPort->rect.w - 15)/2  - w/2;
    mainFile->rect.y= (DrawPort->rect.h - 15)/2  - h/2;

    SDL_BlitSurface(mainFile->layer->surface,NULL,mainFile->surface,NULL);

    DF_UpdateFile();


    return 1;
}

void DF_CreateScrollDrawing()
{
    SDL_Rect vertical, horizontal;

    //scroll bar background
    vertical.x=DrawPort->rect.w - 15;
    vertical.y=0;
    vertical.w=15;
    vertical.h=DrawPort->rect.h - 15;

    horizontal.x=0;
    horizontal.y=DrawPort->rect.h - 15;
    horizontal.w=DrawPort->rect.w - 15;
    horizontal.h=15;

    SDL_FillRect(DrawPort->surface,&vertical,SYS_RGBA(0x40,0x40,0x40,0xff));
    SDL_FillRect(DrawPort->surface,&horizontal,SYS_RGBA(0x40,0x40,0x40,0xff));

    //scroll bar handler
    vertical.h=  ((DrawPort->rect.h - 15) * (DrawPort->rect.h - 15) ) / (mainFile->rect.h + DrawPort->rect.h -15);
    vertical.y=  (((DrawPort->rect.h - 15)/2 -mainFile->rect.y)*(DrawPort->rect.h-15)) / (mainFile->rect.h + DrawPort->rect.h -15);

    horizontal.w=((DrawPort->rect.w - 15) * (DrawPort->rect.w - 15) ) / (mainFile->rect.w + DrawPort->rect.w -15);
    horizontal.x=(((DrawPort->rect.w - 15)/2 -mainFile->rect.x)*(DrawPort->rect.w-15)) / (mainFile->rect.w + DrawPort->rect.w -15);

    if(vertical.h<DrawPort->rect.h)
    {
        SDL_FillRect(DrawPort->surface,&vertical,SYS_RGBA(0x70,0x70,0x70,0xff));
    }
    if(horizontal.w<DrawPort->rect.w)
    {
        SDL_FillRect(DrawPort->surface,&horizontal,SYS_RGBA(0x70,0x70,0x70,0xff));
    }

}

void DF_CheckFileRect()
{


    if(mainFile->rect.x > (DrawPort->rect.w - 15)/2)
    {
        mainFile->rect.x = (DrawPort->rect.w - 15)/2;
    }
    if(mainFile->rect.y > (DrawPort->rect.h - 15)/2)
    {
        mainFile->rect.y = (DrawPort->rect.h - 15)/2;
    }
    if(mainFile->rect.x + mainFile->rect.w < (DrawPort->rect.w - 15)/2)
    {
        mainFile->rect.x = (DrawPort->rect.w - 15)/2 - mainFile->rect.w;
    }
    if(mainFile->rect.y + mainFile->rect.h < (DrawPort->rect.h - 15)/2)
    {
        mainFile->rect.y = (DrawPort->rect.h - 15)/2 - mainFile->rect.h;
    }


}

void DF_UpdateFile()
{
    SDL_Rect rect;//saving main file rect from editing

    DF_Layer*currentLayer=mainFile->layer;

    rect.x=mainFile->rect.x;
    rect.y=mainFile->rect.y;
    rect.w=mainFile->rect.w;
    rect.h=mainFile->rect.h;

    SDL_FillRect(mainFile->surface,NULL,0);

    while(currentLayer)
    {
        if(currentLayer->view)
            SDL_BlitSurface(currentLayer->surface,NULL,mainFile->surface,NULL);
        currentLayer=currentLayer->next;
    }

    SDL_FillRect(DrawPort->surface,NULL,DrawPort->viewport->bgcolor);
    SDL_FillRect(DrawPort->surface,&rect,0);
    if(mainFile->zoom==100)
    {
        SDL_BlitSurface(mainFile->surface,NULL,DrawPort->surface,&rect);
    }
    else
    {
        SDL_BlitScaled(mainFile->surface,NULL,DrawPort->surface,&rect);
    }
    DF_CreateScrollDrawing();
}

void DF_GetMouseFromLayer(int * x, int * y)
{
    *x -= (DrawPort->rect.x +mainFile->rect.x);
    *x  = (*x *100)/mainFile->zoom;
    *y -= (DrawPort->rect.y +mainFile->rect.y);
    *y  = (*y *100)/mainFile->zoom;

}
