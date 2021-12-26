#include "line.h"

#include <SDL.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"
#include "line-plot.h"
#include "toolOption.h"

#include "../DR-viewport.h"
#include "../DR-vars.h"
#include "../file/file.h"
#include "../file/layer.h"
#include "../file/drawable.h"
#include "../core/sys.h"
#include "../core/color.h"

int lineSize = 15;

int TOOL_SetLine(DR_Tools * line)
{
    SDL_Rect boxRect;
    line->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
    if(line->cursor==NULL)
    {
        return 0;
    }
    /*line size*/
    boxRect.x=5;
    boxRect.y=0;
    boxRect.w=150;
    boxRect.h=30;
    TOOLOPTION_CreateOptionSet("size",TOOL_SET_SCROLLBOX,TOOL_SetValueSize);

    TOOLOPTION_CreateScrollBox("size",lineSize,1000,"Size","px",&boxRect,SCRL_SQRT,TOOL_BoxSetValueSize);

    TOOLOPTION_CreateOptionBar();

    return 1;
}

void TOOL_UnSetLine(DR_Tools * line)
{
    SDL_FreeCursor(line->cursor);
    line->cursor=NULL;
    TOOLOPTION_DestroyOptionBar();
}

void TOOL_UseLine(int x,int y)
{
    int i,j;
    SDL_Event event;
    SDL_Rect srect,drect;
    SDL_Texture * tempTexture=NULL;
    SDL_Surface * tempSurface=SDL_CreateRGBSurface(0, mainFile->w, mainFile->h, 32, rmask, gmask, bmask, amask);
    if(tempSurface==NULL)
    {
        return;
    }

   DF_GetMouseFromLayer(&x,&y);

    while(SDL_WaitEvent(&event))
    {
        if(event.type==SDL_MOUSEMOTION)
        {
            SDL_FillRect(tempSurface,NULL,0);

            SDL_GetMouseState(&i,&j);

            DF_GetMouseFromLayer(&i,&j);

            TOOL_DrawLine(tempSurface,x,y,i,j,lineSize);

            DR_SetBufferToDrawableData(tempSurface);

            if(tempTexture)
            {
                SDL_DestroyTexture(tempTexture);
            }

            tempTexture=TOOL_CreateLineTexture(tempSurface,&srect,&drect);



        }
        else if(event.type==SDL_MOUSEBUTTONUP)
        {
           // TOOL_DrawLine(tempSurface,x,y,i,j,5);
            DR_SetDrawableBuffer(tempSurface);
            DR_BlitDrawable();
            break;
        }

        //manage veiwports
        DR_MainViewPort(renderer, &VP_Texture);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,BGTexture,NULL,NULL);
        SDL_RenderCopy(renderer,VP_Texture,NULL,NULL);

        SDL_RenderCopy(renderer,tempTexture,&srect,&drect);
        if(GLOBAL_SELECTION_LINES)
        {
            SDL_RenderCopy(renderer,GLOBAL_SELECTION_LINES,&GLOBAL_SELECTION_LINES_SOURCE_Rect,&GLOBAL_SELECTION_LINES_DEST_Rect);
        }
        SDL_RenderPresent(renderer);
    }

    SDL_FreeSurface(tempSurface);
    SDL_DestroyTexture(tempTexture);

}

void TOOL_DrawLine(SDL_Surface * surface,double x0,double y0,double x1,double y1,int size)
{
    float dx,dy;
    float gradient;
    float xend,yend;
//    float xgap;
    float xpx11;//ypx11;
    float xpxl2;//ypx12;
    float intery;
    int x,i;

    char steep = abs(y1 - y0) > abs(x1 - x0);

    if(steep)
    {
        LINE_Swap(&x0, &y0);
        LINE_Swap(&x1, &y1);
    }
    if(x0>x1)
    {
        LINE_Swap(&x0, &x1);
        LINE_Swap(&y0, &y1);
    }

    dx=x1-x0;
    dy=y1-y0;


    if(dx==0)
    {
        gradient=1;
    }
    else
    {
        gradient= dy/dx;
    }

    // handle first endpoint
    xend = LINE_Round(x0);
    yend = y0 + gradient * (xend - x0);

  //  xgap = LINE_RFPart(x0 + 0.5);
    xpx11 = xend;
 //   ypx11 = LINE_IPart(yend);
/*
    if(steep)
    {
        LINE_PlotPoint(surface, ypx11, xpx11, LINE_RFPart(yend) * xgap);
        LINE_PlotPoint(surface, ypx11 + 1, xpx11, LINE_FPart(yend) * xgap);
    }
    else
    {
        LINE_PlotPoint(surface, xpx11, ypx11, LINE_RFPart(yend) * xgap);
        LINE_PlotPoint(surface, xpx11, ypx11 + 1, LINE_FPart(yend) * xgap);
    }
*/
    intery = yend + gradient; // first y-intersection for the main loop

    // handle second endpoint
    xend = LINE_Round(x1);
    yend = y1 + gradient * (xend - x1);
   // xgap = LINE_FPart(x1 + 0.5);

    xpxl2 = xend; //this will be used in the main loop
//    ypx12 = LINE_IPart(yend);
/*
    if(steep)
    {
        LINE_PlotPoint(surface, ypx12, xpxl2, LINE_RFPart(yend) * xgap);
        LINE_PlotPoint(surface, ypx12+1, xpxl2, LINE_RFPart(yend) * xgap);
    }
    else
    {

        LINE_PlotPoint(surface, xpxl2, ypx12, LINE_RFPart(yend) * xgap);
        LINE_PlotPoint(surface, xpxl2, ypx12+1, LINE_RFPart(yend) * xgap);
    }
*/
    if(steep)
    {
        for(x=xpx11+1 ; x<xpxl2; x++)
        {
            //inside pixels
            for(i=-size/2;i<size/2;i++)
            {
                LINE_PlotPoint(surface, LINE_IPart(intery)+i, x, 1);
            }
            //border
            LINE_PlotPoint(surface, LINE_IPart(intery)-size/2, x, LINE_RFPart(intery));
            LINE_PlotPoint(surface, LINE_IPart(intery) + size/2, x, LINE_FPart(intery));

            intery += gradient;
        }
    }
    else
    {
        for(x=xpx11+1 ; x<xpxl2; x++)
        {

            //inside pixels
            for(i=-size/2;i<size/2;i++)
            {
                LINE_PlotPoint(surface, x, LINE_IPart(intery)+i, 1);
            }
            //border
            LINE_PlotPoint(surface, x, LINE_IPart(intery)-size/2, LINE_RFPart(intery));
            LINE_PlotPoint(surface, x, LINE_IPart(intery) + size/2, LINE_FPart(intery));

            intery += gradient;
        }
    }
}

void TOOL_SetValueSize(int value)
{
    lineSize = value;
}
void TOOL_BoxSetValueSize( int value)
{
    TOOL_SetValueSize(value);
}
