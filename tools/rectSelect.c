#include "rectSelect.h"
#include "tools.h"

#include <SDL.h>
#include <SDL_image.h>

#include "../DR-vars.h"
#include "../DR-viewport.h"
#include "../core/sys.h"
#include "../file/file.h"
#include "../file/selection.h"

int TOOL_SetRectSelect(DR_Tools * select)
{
    SDL_Surface * cursor=IMG_Load("cursor\\rect-select.png");
    if(cursor==NULL)
    {
        return 0;
    }
    select->cursor=SDL_CreateColorCursor(cursor,11,11);
    if(select->cursor==NULL)
    {
        return 0;
    }
    SDL_FreeSurface(cursor);

    return 1;
}

void TOOL_UnsSetRectSelect(DR_Tools * select)
{
    SDL_FreeCursor(select->cursor);
    select->cursor=NULL;
}

void TOOL_RectSelect(int x,int y)
{
    SDL_Surface * tempSurface=NULL;
    SDL_Texture * tempTexture=NULL;

    SDL_Event event;
    SDL_Rect srect,drect;

    int i,j;
    DR_MainViewport * currentPort=DR_FindViewport("drawp");
    int x0,y0;

    x0=currentPort->rect.x+mainFile->rect.x;
    y0=currentPort->rect.y+mainFile->rect.y;

    x-=x0;
    y-=y0;
    //save from one click
    i=x;
    j=y;


    tempSurface = SDL_CreateRGBSurface(0,mainFile->w,mainFile->h,32,rmask,gmask,bmask,amask);
    if(tempSurface==NULL)
    {
        return;
    }

    while(SDL_WaitEvent(&event))
    {
        if(event.type==SDL_MOUSEMOTION)
        {
            SDL_FillRect(tempSurface,NULL,0);
            SDL_GetMouseState(&i,&j);
            i-=x0;
            j-=y0;

            TOOL_DrawSelectionGuide(tempSurface,x,y,i,j);
            if(tempTexture)
            {
                SDL_DestroyTexture(tempTexture);
            }

            tempTexture=TOOL_CreateLineTexture(tempSurface,&srect,&drect);
        }
        else if(event.type==SDL_MOUSEBUTTONUP)
        {
            //make selection first then break
            if(i!=x && j!=y)//save from one click
            {
                TOOL_DrawSelectionRect(tempSurface,x,y,i,j);
            }
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
}

void TOOL_DrawSelectionGuide(SDL_Surface * surface,int x,int y,int i,int j)
{
    int a,t;
    if(x>i)
    {
        t=x;
        x=i;
        i=t;
    }
    if(y>j)
    {
        t=y;
        y=j;
        j=t;
    }
    if(x<0) x=0;
    if(y<0) y=0;
    if(i<0) i=0;
    if(j<0) j=0;

    if(x>surface->w-1) x=surface->w-1;
    if(y>surface->h-1) y=surface->h-1;
    if(i>surface->w-1) i=surface->w-1;
    if(j>surface->h-1) j=surface->h-1;

    for(a=x;a<=i;a++)
    {
        MZSDL_PutPixel32(surface,a,y,SYS_RGBA(0xff,0,0,0xff));
        MZSDL_PutPixel32(surface,a,j,SYS_RGBA(0xff,0,0,0xff));
    }

    for(a=y;a<=j;a++)
    {

        MZSDL_PutPixel32(surface,x,a,SYS_RGBA(0xff,0,0,0xff));
        MZSDL_PutPixel32(surface,i,a,SYS_RGBA(0xff,0,0,0xff));
    }

}
int TOOL_DrawSelectionRect(SDL_Surface *surface,int x,int y,int i,int j)
{
    SL_Point p1,p2,p3,p4;

    if(x<0) x=0;
    if(y<0) y=0;
    if(i>mainFile->w-1) i=mainFile->w-1;
    if(j>mainFile->h-1) j=mainFile->h-1;

    p1.x=x;
    p1.y=y;

    p2.x=x;
    p2.y=j;

    p3.x=i;
    p3.y=j;

    p4.x=i;
    p4.y=y;

    DR_AddSelectionLine(&p1,&p2);
    DR_AddSelectionLine(&p2,&p3);
    DR_AddSelectionLine(&p3,&p4);
    DR_AddSelectionLine(&p4,&p1);

    DR_CreateSelection();

    return 1;
}
