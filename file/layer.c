#include "layer.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <SDL.h>

#include "../core/sys.h"



int DF_CreateLayer(DF_File * File,  int color)
{
    DF_Layer *layer=NULL;
    //DF_Layer * currentLayer=NULL;
    char layernumber[8];

    layer= (DF_Layer *) malloc(sizeof(DF_Layer));
    if(layer==NULL)
    {
        return 0;
    }
    layer->surface=SDL_CreateRGBSurface(0,File->w,File->h,32,rmask,gmask,bmask,amask);
    if(layer->surface==NULL)
    {
        return 0;
    }

    layer->lockPaint=0;
    layer->lockAlpha=0;
    layer->view=1;
    layer->opacity=100;

    layer->next=NULL;

    if(SDL_FillRect(layer->surface,NULL,color)==-1)
    {
        printf("Error:%s\n",SDL_GetError());
        return -1;
    }

    strcpy(layer->name,"Layer ");
    itoa(++mainFile->layerCount,layernumber,10);
    strcat(layer->name,layernumber);

    if(File->layer==NULL)
    {
        File->layer=layer;
    }
    else
    {
        layer->next=File->activeLayer->next;
        File->activeLayer->next=layer;
    }

    File->activeLayer=layer;

    return 1;
}

void DF_DeleteLayer(DF_File * File)
{
    DF_Layer * tempLayer=File->layer,*temp2;
    if(File->activeLayer==File->layer)
    {
        File->layer=File->layer->next;
        SDL_FreeSurface(File->activeLayer->surface);
        free(File->activeLayer);
        File->activeLayer=File->layer;
        if(File->layer==NULL)
        {
            DF_CreateLayer(File, 0);
        }
    }
    else
    {
       while(tempLayer->next!=File->activeLayer)
        {
            tempLayer=tempLayer->next;
        }
        temp2=File->activeLayer->next;

        SDL_FreeSurface(File->activeLayer->surface);
        free(File->activeLayer);

        tempLayer->next=temp2;
        File->activeLayer=tempLayer;
    }
}
