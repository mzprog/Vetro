#include "hand.h"

#include <stdlib.h>

#include <SDL.h>
#include <SDL_image.h>
#include "../file/file.h"
#include "../DR-vars.h"
#include "../DR-viewport.h"
#include "tools.h"


SDL_Cursor * handCursor;
SDL_Cursor * handRockCursor;

int TOOL_SetHand(DR_Tools * hand)
{
    SDL_Surface * handSurface;
    SDL_Surface * handRockSurface;

    handSurface= IMG_Load("cursor\\hand-small.png");
    if(handSurface==NULL)
    {
        return 0;
    }
    handRockSurface=IMG_Load("cursor\\hand-rock-small.png");
    if(handRockSurface==NULL)
    {
        return 0;
    }

    handCursor=SDL_CreateColorCursor(handSurface,0,0);
    handRockCursor=SDL_CreateColorCursor(handRockSurface,0,0);

    if(handCursor==NULL)
    {
        return 0;
    }
    hand->cursor=handCursor;

    //SDL_SetCursor(handCursor);

    SDL_FreeSurface(handSurface);
    SDL_FreeSurface(handRockSurface);

    return 1;
}

void TOOL_UnSetHand(DR_Tools * hand)
{
    SDL_FreeCursor(hand->cursor);
    hand->cursor=NULL;

    SDL_FreeCursor(handCursor);
    SDL_FreeCursor(handRockCursor);

    handCursor=NULL;
    handRockCursor=NULL;
}

void TOOL_MoveHand(int x,int y)
{
    SDL_Event *event;
    SDL_Rect oldRect;
    int a,b;
    oldRect.x=mainFile->rect.x;
    oldRect.y=mainFile->rect.y;

    SDL_SetCursor(handRockCursor);

    event=(SDL_Event *) malloc(sizeof(SDL_Event));
    if(event==NULL)
    {
        return;
    }

    while(SDL_WaitEvent(event))
    {
        if(event->type==SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&a,&b);
            mainFile->rect.x = oldRect.x + a - x;
            mainFile->rect.y = oldRect.y + b - y;

            DF_CheckFileRect();
            DF_UpdateFile();
        }
        else if(event->type==SDL_MOUSEBUTTONUP)
        {
            SDL_SetCursor(handCursor);
            break;
        }
        if(GLOBAL_SELECTION_LINES)
        {
            TOOL_UpdateTexture(&GLOBAL_SELECTION_LINES_SOURCE_Rect,&GLOBAL_SELECTION_LINES_DEST_Rect);
        }
        //manage veiwports
        DR_MainViewPort(renderer, &VP_Texture);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,BGTexture,NULL,NULL);
        SDL_RenderCopy(renderer,VP_Texture,NULL,NULL);
        if(GLOBAL_SELECTION_LINES)
        {
            if(SDL_RenderCopy(renderer,GLOBAL_SELECTION_LINES,&GLOBAL_SELECTION_LINES_SOURCE_Rect,&GLOBAL_SELECTION_LINES_DEST_Rect)!=0)puts("renderEror");
            //printf("hint:%s\n",SDL_GetHint(NULL));
            //printf("srg x=%d\ty=%d\tw=%d\th=%d\n",GLOBAL_SELECTION_LINES_SOURCE_Rect.x,GLOBAL_SELECTION_LINES_SOURCE_Rect.y,GLOBAL_SELECTION_LINES_SOURCE_Rect.w,GLOBAL_SELECTION_LINES_SOURCE_Rect.h);
            //printf("deg x=%d\ty=%d\tw=%d\th=%d\n",GLOBAL_SELECTION_LINES_DEST_Rect.x,GLOBAL_SELECTION_LINES_DEST_Rect.y,GLOBAL_SELECTION_LINES_DEST_Rect.w,GLOBAL_SELECTION_LINES_DEST_Rect.h);
        }
        SDL_RenderPresent(renderer);
    }
    free(event);

}
