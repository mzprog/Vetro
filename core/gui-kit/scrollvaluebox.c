#include "scrollvaluebox.h"

//#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../sys.h"
#include "../../navigator/navigator.h"
#include "../../DR-vars.h"
#include "../../DR-viewport.h"


KIT_ScrollValueBox * KIT_CreateScrollValueBox(int value, int maxValue,const char * name, const char * unit, SDL_Rect * rect, int flag, void (*SetValue)(int))
{
    TTF_Font * gFont;
    SDL_Color white={0xff,0xff,0xff};
    SDL_Surface * textSurface=NULL;
    SDL_Rect textRect;
    SDL_Rect barRect;
    char namechar[20];
    char valuechar[8];

    if(flag!= SCRL_LINEAR && flag != SCRL_SQRT)
    {
        return NULL;
    }

    KIT_ScrollValueBox * box= (KIT_ScrollValueBox *) malloc(sizeof(KIT_ScrollValueBox));
    if(box==NULL)
    {
        return NULL;
    }
    box->surface = SDL_CreateRGBSurface(0,rect->w,rect->h,32,rmask,gmask,bmask,amask);
    if(box->surface==NULL)
    {
        free(box);
        return NULL;
    }
    box->flag=flag;

    gFont = TTF_OpenFont( "segoeui.ttf", 12 );
    if(gFont==NULL)
    {
        return 0;
    }
    itoa(value,valuechar,10);
    strcpy(namechar,name);
    strcat(namechar,": ");
    strcat(namechar,valuechar);
    strcat(namechar,unit);

    textSurface = TTF_RenderText_Blended(gFont,namechar,white);

    TTF_CloseFont(gFont);

    textRect.x=rect->w/2 - textSurface->w/2;
    textRect.y=rect->h/2 - textSurface->h/2;
    textRect.w=textSurface->w;
    textRect.h=textSurface->h;

    SDL_FillRect(box->surface,NULL,SYS_RGBA(0x7f,0x7f,0x7f,0xff));
    SYS_DrawVerticalLine(box->surface,0,0,rect->h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
    SYS_DrawVerticalLine(box->surface,rect->w-1,0,rect->h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));

    SYS_DrawHorizantalLine(box->surface,0,rect->w-1,0,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
    SYS_DrawHorizantalLine(box->surface,0,rect->w-1,rect->h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));

    barRect.x=1;
    barRect.y=1;
    barRect.h=rect->h-2;
    if(box->flag==SCRL_LINEAR)
    {
        barRect.w=(rect->w-2)*value/maxValue;
    }
    else if(box->flag==SCRL_SQRT)
    {
        barRect.w=(rect->w-2)*sqrt(1.0*value/maxValue);
    }

    SDL_FillRect(box->surface,&barRect,SYS_RGBA(0x3f,0x5f,0x7f,0xff));
    SDL_BlitSurface(textSurface,NULL,box->surface,&textRect);

    //copying data
    box->maxValue=maxValue;
    strcpy(box->name,name);
    strcpy(box->unit,unit);
    box->SetValue=SetValue;
    box->value=value;

    box->rect.x=rect->x;
    box->rect.y=rect->y;
    box->rect.w=rect->w;
    box->rect.h=rect->h;

    //return
    return box;
}

void KIT_DestroyScrollValueBox(KIT_ScrollValueBox * box)
{
    SDL_FreeSurface(box->surface);
    free(box);
}

void KIT_ScrollValueBoxEvent(KIT_ScrollValueBox * box, NV_Docker *docker, NV_Panel * panel,int x,int y)
{
    int i,Xbefore;
    SDL_Event event;

    Xbefore  =  panel->rect.x + docker->rect.x +box->rect.x;
    x -= Xbefore;

    Xbefore += NV_GetNavBarPort()->rect.x;
    KIT_ScrollValueBoxDrawEvent(box,docker,panel,x,0);
    box->SetValue(box->value);
    //render everything
    DR_MainViewPort(renderer, &VP_Texture);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,BGTexture,NULL,NULL);
    SDL_RenderCopy(renderer,VP_Texture,NULL,NULL);
    if(GLOBAL_SELECTION_LINES)
    {
        SDL_RenderCopy(renderer,GLOBAL_SELECTION_LINES,&GLOBAL_SELECTION_LINES_SOURCE_Rect,&GLOBAL_SELECTION_LINES_DEST_Rect);
    }
    SDL_RenderPresent(renderer);

    while(SDL_WaitEvent(&event))
    {
        if(event.type==SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&i,NULL);
            i-=Xbefore;

            KIT_ScrollValueBoxDrawEvent(box,docker,panel,i,0);
            box->SetValue(box->value);
        }
        else if(event.type==SDL_MOUSEBUTTONUP)
        {
            break;
        }

        DR_MainViewPort(renderer, &VP_Texture);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,BGTexture,NULL,NULL);
        SDL_RenderCopy(renderer,VP_Texture,NULL,NULL);
        if(GLOBAL_SELECTION_LINES)
        {
            SDL_RenderCopy(renderer,GLOBAL_SELECTION_LINES,&GLOBAL_SELECTION_LINES_SOURCE_Rect,&GLOBAL_SELECTION_LINES_DEST_Rect);
        }
        SDL_RenderPresent(renderer);
    }
}

void KIT_ScrollValueBoxDrawEvent(KIT_ScrollValueBox * box, NV_Docker *docker, NV_Panel * panel,int x,int y)
{
    SDL_Rect barRect;
    SDL_Rect textRect;
    TTF_Font * gFont;
    SDL_Color white={0xff,0xff,0xff};
    SDL_Surface * textSurface=NULL;
    char namechar[20];
    char valuechar[8];

    gFont = TTF_OpenFont( "segoeui.ttf", 12 );
    if(gFont==NULL)
    {
        return;
    }

    SDL_FillRect(box->surface,NULL,SYS_RGBA(0x7f,0x7f,0x7f,0xff));
    SYS_DrawVerticalLine(box->surface,0,0,box->rect.h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
    SYS_DrawVerticalLine(box->surface,box->rect.w-1,0,box->rect.h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));

    SYS_DrawHorizantalLine(box->surface,0,box->rect.w-1,0,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
    SYS_DrawHorizantalLine(box->surface,0,box->rect.w-1,box->rect.h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));

    barRect.x=1;
    barRect.y=1;
    //barRect.w=(rect->w-2)*value/maxValue;
    barRect.w=x;
    if(x<0)
    {
        barRect.w=0;
    }
    else if(x>box->rect.w-2)
    {
        barRect.w=box->rect.w-2;
    }
    barRect.h=box->rect.h-2;
    SDL_FillRect(box->surface,&barRect,SYS_RGBA(0x3f,0x5f,0x7f,0xff));

    //box->value=(box->maxValue*barRect.w)/(box->rect.w-2);
    if(box->flag==SCRL_LINEAR)
    {
        box->value=(box->maxValue*barRect.w)/(box->rect.w-2);
    }
    else if(box->flag==SCRL_SQRT)
    {
        box->value= box->maxValue * pow(barRect.w / (box->rect.w-2.0), 2);
    }


    itoa(box->value,valuechar,10);
    strcpy(namechar,box->name);
    strcat(namechar,": ");
    strcat(namechar,valuechar);
    strcat(namechar,box->unit);

    textSurface = TTF_RenderText_Blended(gFont,namechar,white);


    textRect.x=box->rect.w/2 - textSurface->w/2;
    textRect.y=box->rect.h/2 - textSurface->h/2;
    textRect.w=textSurface->w;
    textRect.h=textSurface->h;

    SDL_BlitSurface(textSurface,NULL,box->surface,&textRect);

    SDL_BlitSurface(box->surface,NULL,docker->surface,&box->rect);
    SDL_BlitSurface(docker->surface,NULL,panel->surface,&docker->rect);
    SDL_BlitSurface(panel->surface,NULL,NV_GetNavBarPort()->surface,&panel->rect);

    SDL_FreeSurface(textSurface);
    TTF_CloseFont(gFont);
}

void KIT_ScrollValueBoxSetValue(KIT_ScrollValueBox * box,int i)
{
    TTF_Font * gFont;
    SDL_Color white={0xff,0xff,0xff};
    SDL_Surface * textSurface=NULL;
    SDL_Rect textRect;
    SDL_Rect barRect;
    char namechar[20];
    char valuechar[8];

    gFont = TTF_OpenFont( "segoeui.ttf", 12 );

     if(gFont==NULL)
    {
        return;
    }
    itoa(i,valuechar,10);
    strcpy(namechar,box->name);
    strcat(namechar,": ");
    strcat(namechar,valuechar);
    strcat(namechar,box->unit);

    textSurface = TTF_RenderText_Blended(gFont,namechar,white);

    TTF_CloseFont(gFont);

    textRect.x=box->rect.w/2 - textSurface->w/2;
    textRect.y=box->rect.h/2 - textSurface->h/2;
    textRect.w=textSurface->w;
    textRect.h=textSurface->h;

    SDL_FillRect(box->surface,NULL,SYS_RGBA(0x7f,0x7f,0x7f,0xff));
    SYS_DrawVerticalLine(box->surface,0,0,box->rect.h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
    SYS_DrawVerticalLine(box->surface,box->rect.w-1,0,box->rect.h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));

    SYS_DrawHorizantalLine(box->surface,0,box->rect.w-1,0,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
    SYS_DrawHorizantalLine(box->surface,0,box->rect.w-1,box->rect.h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));

    barRect.x=1;
    barRect.y=1;
    //barRect.w=(box->rect.w-2)*i/box->maxValue;
    barRect.h=box->rect.h-2;

    if(box->flag==SCRL_LINEAR)
    {
        barRect.w=(box->rect.w-2)*i/box->maxValue;
    }
    else if(box->flag==SCRL_SQRT)
    {
        barRect.w=(box->rect.w-2)*sqrt(1.0*i/box->maxValue);
    }

    SDL_FillRect(box->surface,&barRect,SYS_RGBA(0x3f,0x5f,0x7f,0xff));
    SDL_BlitSurface(textSurface,NULL,box->surface,&textRect);

}
