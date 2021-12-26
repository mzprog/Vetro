#include "selectoption.h"

#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "../sys.h"
#include "../../tools/line.h"
#include "gui-tools.h"

KIT_SelectOption * KIT_CreateSelectOption(char ** options,int size ,SDL_Rect * rect)
{
    int i,width,height;
    SDL_Rect textRect;
    KIT_SelectOption * box=(KIT_SelectOption *) malloc(sizeof(KIT_SelectOption));
    if(box==NULL)
    {
        return NULL;
    }

    box->surface= SDL_CreateRGBSurface(0,rect->w,rect->h,32,rmask,gmask,bmask,amask);
    if(box->surface==NULL)
    {
        free(box);
        return NULL;
    }

    box->option=NULL;

    for(i=0;i<size;i++)
    {
        KIT_AddOption(box,options[i]);
    }

    width=KIT_GetMaxOptionWidth(box);
    height=box->option->rect.h*size;//maybe we will get the function to count indivisual

    box->listSurface=SDL_CreateRGBSurface(0,width,height,32,rmask,gmask,bmask,amask);
    if(box->listSurface==NULL)
    {
//free everything
        return NULL;
    }

    box->listRect.w=width;
    box->listRect.h=height;
    //draw list
//hjghggg error
//
    SDL_FillRect(box->listSurface,NULL,SYS_RGBA(0xdf,0xdf,0xdf,0xff));
    SYS_DrawVerticalLine(box->surface,0,0,rect->h-1,SYS_RGBA(0xdf,0xdf,0xdf,0xff));
    SYS_DrawVerticalLine(box->surface,rect->w-1,0,rect->h-1,SYS_RGBA(0xdf,0xdf,0xdf,0xff));

    SYS_DrawHorizantalLine(box->surface,0,rect->w-1,0,SYS_RGBA(0xdf,0xdf,0xdf,0xff));
    SYS_DrawHorizantalLine(box->surface,0,rect->w-1,rect->h-1,SYS_RGBA(0xdf,0xdf,0xdf,0xff));

/*
/fffffffffffffffff
/
/ssdfdddddddddddddddd
//dfgdsadfdsaaaaaaaaaasaaaaad
/
/dfdddddddd
/
*/

    box->rect.x=rect->x;
    box->rect.y=rect->y;
    box->rect.h=rect->h;
    box->rect.w=rect->w;

    SDL_FillRect(box->surface,NULL,SYS_RGBA(0x7f,0x7f,0x7f,0xff));
    SYS_DrawVerticalLine(box->surface,0,0,rect->h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
    SYS_DrawVerticalLine(box->surface,rect->w-1,0,rect->h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));

    SYS_DrawHorizantalLine(box->surface,0,rect->w-1,0,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
    SYS_DrawHorizantalLine(box->surface,0,rect->w-1,rect->h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));



    KIT_DrawBlendedLine(box->surface,box->rect.w-16,9,box->rect.w-10,15,SYS_RGBA(0xff,0xff,0xff,0xff));
    KIT_DrawBlendedLine(box->surface,box->rect.w-11,15,box->rect.w-5,9,SYS_RGBA(0xff,0xff,0xff,0xff));
    //printf("width=%d\n",box->rect.w);

    textRect.x=box->selectedOption->textRect.x;
    textRect.y=box->selectedOption->textRect.y%size;
    textRect.w=box->selectedOption->textRect.w;
    textRect.h=box->selectedOption->textRect.h;

    SDL_BlitSurface(box->selectedOption->surface,NULL,box->surface,&textRect);
    //return
    return box;
}

int KIT_AddOption(KIT_SelectOption *box, const char * option)
{
    TTF_Font * gFont;
    SDL_Color black={0xff,0xff,0xff};
    KIT_Option * curOpt=box->option;
    KIT_Option * opt=(KIT_Option *) malloc(sizeof(KIT_Option));
    if(opt==NULL)
    {
        return NULL;
    }

    gFont = TTF_OpenFont( "segoeui.ttf", 12 );
     if(gFont==NULL)
    {
        return 0;
    }

    opt->next=NULL;

    strcpy(opt->name,option);

    opt->surface=TTF_RenderText_Blended(gFont,opt->name,black);

    if(opt->surface==NULL)
    {
        free(opt);
        return NULL;
    }

    TTF_CloseFont(gFont);

    opt->textRect.x=10;
    opt->textRect.w=opt->surface->w;
    opt->textRect.h=opt->surface->h;

    opt->rect.x=0;
    opt->rect.h=opt->textRect.h+10;

    if(box->option==NULL)
    {
        box->option=opt;
        box->selectedOption=opt;
        opt->ID=1;

        opt->rect.y=0;

        opt->textRect.y=3;
    }
    else
    {
        while(curOpt->next)
        {
            curOpt=curOpt->next;
        }
        curOpt->next=opt;
        opt->ID=curOpt->ID+1;

        opt->rect.y=curOpt->rect.y+curOpt->rect.h;
        opt->textRect.y=opt->rect.y+5;
    }

    return 1;
}

int KIT_GetMaxOptionWidth(KIT_SelectOption *box)
{
    int w=0;
    KIT_Option * current=box->option;

    while(current)
    {
        if(w<current->textRect.w)
            w=current->textRect.w;
        current=current->next;
    }
    current=box->option;
    while(current)
    {
        current->rect.w=w;
        current=current->next;
    }
    return w+20;
}
