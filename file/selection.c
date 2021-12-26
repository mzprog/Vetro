#include "selection.h"

#include <SDL.h>
#include <stdlib.h>

#include "selectionTools.h"

#include "../core/sys.h"
#include "file.h"
#include "../DR-vars.h"
#include "drawable.h"
#include "../tools/tools.h"

int selectMode=0;
SDL_Surface * selectionArea=NULL;

SL_Line * PolygonHead=NULL;


int DR_InitSelection()
{
    selectionArea=SDL_CreateRGBSurface(0,mainFile->w,mainFile->h,32,rmask,gmask,bmask,amask);
    if(selectionArea==NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int DR_GetSelectMode()
{
    return selectMode;
}
void DR_SetSelectMode(int mode)
{
    selectMode=mode;
}

SL_Line * DR_GetPolygonHead()
{
    return PolygonHead;
}


int DR_AddSelectionLine(SL_Point *p1, SL_Point * p2)
{
    SL_Line *CurrentLine=PolygonHead;
    SL_Line * line= (SL_Line *) malloc(sizeof(SL_Line));
    if(line==NULL)
    {
        return 0;
    }

    line->p1.x=p1->x;
    line->p1.y=p1->y;

    line->p2.x=p2->x;
    line->p2.y=p2->y;
    line->next=NULL;

    if(PolygonHead==NULL)
    {
        PolygonHead=line;
    }
    else
    {
        while(CurrentLine->next)
        {
            CurrentLine=CurrentLine->next;
        }
        CurrentLine->next=line;
    }

    return 1;
}

int DR_CreateSelection()
{
    SL_Point pi,pw;
    SL_Point p;

    int i,j;
    int x0,x;
    int minx,maxx,miny,maxy;

    minx=SL_FindMinimumX() - 1;
    maxx=SL_FindMaximumX() + 1;

    miny=SL_FindMinimumY() - 1;
    maxy=SL_FindMaximumY() + 1;

    DR_FillDrawable(0);

    for(j=miny;j<maxy;j++)
    {

        pi.x=minx;
        pi.y=j;
        pw.x=maxx;
        pw.y=j;
        p.y=j;

        while(SL_FindFirstInterSection(&x0,&pi,&pw))
        {

            p.x=x0+1;

            if(SL_FindFirstInterSection(&x,&p,&pw)==0)
            {
                x=maxx;
            }
            pi.x=x;
            for(i=x0; i<x; i++)
            {

                DR_SetDrawbleXY(i,j,0xff);
            }
        }

    }
    DR_SetSelectMode(1);
    DR_ClearLines();
    DR_DrawSelectionArea();

    return 1;
}


void DR_ClearLines()
{
    SL_Line * current, * current2;
    current=PolygonHead;
    while(current)
    {
        current2=current;
        current=current->next;
        free(current2);
    }
    PolygonHead=NULL;
}

int DR_DrawSelectionArea()
{
    int i,j;
    int inside;
    selectionArea = SDL_CreateRGBSurface(0,mainFile->w,mainFile->h,32,rmask,gmask,bmask,amask);
    if(selectionArea==NULL)
    {
        return 0;
    }
for(i=0;i<selectionArea->w;i++)
    {
        MZSDL_PutPixel32(selectionArea,i,0,SYS_RGBA(0,255,0,255));
        MZSDL_PutPixel32(selectionArea,i,selectionArea->h-1,SYS_RGBA(0,255,0,255));
    }
    for(i=0;i<selectionArea->h;i++)
    {
        MZSDL_PutPixel32(selectionArea,0,i,SYS_RGBA(0,255,0,255));
        MZSDL_PutPixel32(selectionArea,selectionArea->w-1,i,SYS_RGBA(0,255,0,255));
    }

    //horizontal scan
    for(i=0;i<selectionArea->w;i++)
    {
        inside=0;

        if(DR_GetDrawableXY(i,selectionArea->w-1)>=0xff)
        {
            MZSDL_PutPixel32(selectionArea,i,selectionArea->w-1,SYS_RGBA(0xff,0,0,0xff));
        }

        for(j=0;j<selectionArea->h;j++)
        {
            if(DR_GetDrawableXY(i,j)>=0x80 && !inside)
            {
                MZSDL_PutPixel32(selectionArea,i,j,SYS_RGBA(0xff,0,0,0xff));
                inside=1;
            }
            else if(DR_GetDrawableXY(i,j)<0x80 && inside)
            {
                MZSDL_PutPixel32(selectionArea,i,j-1,SYS_RGBA(0xff,0,0,0xff));
                inside=0;
            }
        }
    }

    //vertical scan
    for(j=0;j<selectionArea->h;j++)
    {
        inside=0;//temp
        if(DR_GetDrawableXY(selectionArea->h-1,j)>=0xff)
        {
            MZSDL_PutPixel32(selectionArea,selectionArea->h-1,j,SYS_RGBA(0xff,0,0,0xff));
        }
        for(i=0;i<selectionArea->w;i++)
        {
           if(DR_GetDrawableXY(i,j)>=0x80 && !inside)
            {
                MZSDL_PutPixel32(selectionArea,i,j,SYS_RGBA(0xff,0,0,0xff));
                inside=1;
            }
            else if(DR_GetDrawableXY(i,j)<0x80 && inside)
            {
                MZSDL_PutPixel32(selectionArea,i-1,j,SYS_RGBA(0xff,0,0,0xff));
                inside=0;
            }
        }
    }
    if(GLOBAL_SELECTION_LINES)
    {
        SDL_DestroyTexture(GLOBAL_SELECTION_LINES);
    }
    GLOBAL_SELECTION_LINES=TOOL_CreateLineTexture(selectionArea,&GLOBAL_SELECTION_LINES_SOURCE_Rect,&GLOBAL_SELECTION_LINES_DEST_Rect);
    if(GLOBAL_SELECTION_LINES==NULL)
    {
        return 0;
    }



    return 1;
}
