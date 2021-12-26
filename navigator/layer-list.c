#include "layer-list.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h>

#include "../core/sys.h"
#include "../file/file.h"



DK_LayerList * DK_CreateLayerList(SDL_Rect *rect)
{
    SDL_Rect scrollbar;
    DK_LayerList *list=(DK_LayerList *) malloc(sizeof(DK_LayerList));
    if(list==NULL)
    {
        return NULL;
    }
    list->member=NULL;
    list->rect.x=rect->x;
    list->rect.y=rect->y;
    list->rect.w=rect->w;
    list->rect.h=rect->h;

    list->surface=SDL_CreateRGBSurface(0,rect->w,rect->h,32,rmask,gmask,bmask,amask);
    if(list->surface==NULL)
    {
        free(list);
        return NULL;
    }
    SDL_FillRect(list->surface,NULL,SYS_RGBA(0x9f,0x9f,0x9f,0xff));

    SYS_DrawVerticalLine(list->surface,0,0,rect->h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
    SYS_DrawVerticalLine(list->surface,rect->w-1,0,rect->h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));

    SYS_DrawHorizantalLine(list->surface,0,rect->w-1,0,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
    SYS_DrawHorizantalLine(list->surface,0,rect->w-1,rect->h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));

    //scroll bar
    scrollbar.x=rect->w-15;
    scrollbar.y=0;
    scrollbar.w=15;
    scrollbar.h=rect->h;

    SDL_FillRect(list->surface,&scrollbar,SYS_RGBA(0x6f,0x6f,0x6f,0xff));

    DK_CreateLayerListMember(list,rect,rect->w-scrollbar.w);


    return list;
}

int DK_CreateLayerListMember(DK_LayerList *list,SDL_Rect *rect ,int width)
{
    DF_Layer * currentLayer=mainFile->layer;

    while(currentLayer)
    {
        DK_CreateLayerListMember_Single(list,currentLayer,width);
        currentLayer=currentLayer->next;
    }
    DK_CreateLayerListMember_Surface(list);
    SDL_BlitSurface(list->membersSurface,NULL,list->surface,&list->memberRect);

   return 1;
}

int DK_CreateLayerListMember_Single(DK_LayerList *list, DF_Layer *layer, int width)
{
    //SDL_Rect rect;
    SDL_Surface *temp=NULL;
    int scale,max;
    TTF_Font * gFont;
    SDL_Color white={0xff,0xff,0xff,0xff};

    gFont = TTF_OpenFont( "segoeui.ttf", 12 );
     if(gFont==NULL)
    {
        return 0;
    }

    DK_LayerListMember * currentMember=NULL;
    DK_LayerListMember * member=(DK_LayerListMember *) malloc(sizeof(DK_LayerListMember));
    if(member==NULL)
    {
        return 0;
    }
    member->next=NULL;
    member->layer=layer;

    member->rect.x=1;
    member->rect.w=width-1;
    member->rect.h=30;//maybe we will edit it later

    member->surface=SDL_CreateRGBSurface(0,member->rect.w,member->rect.h,32,rmask,gmask,bmask,amask);
    if(member->surface==NULL)
    {
        free(member);
        return 0;
    }
    if(member->layer==mainFile->activeLayer)
    {
        SDL_FillRect(member->surface,NULL,SYS_RGBA(0x5f,0x7f,0x9f,0xff));
    }
    else
    {
        SDL_FillRect(member->surface,NULL,SYS_RGBA(0x7f,0x7f,0x7f,0xff));
    }

    //visible property
    temp=IMG_Load("icons\\eye.png");

    if(temp==NULL)
    {
//puts("unable to create eye icon");
        return 0;
    }
    if(!member->layer->view)
    {
        SDL_SetSurfaceAlphaMod(temp,63);
    }

    //eye rect
    member->eyeRect.w=temp->w;
    member->eyeRect.h=temp->h;
    member->eyeRect.x=5;
    member->eyeRect.y=5;

    SDL_BlitSurface(temp,NULL,member->surface,&member->eyeRect);
    SDL_FreeSurface(temp);

    //bottom line
    SYS_DrawHorizantalLine(member->surface,0,member->rect.w-1,member->rect.h-1,SYS_RGBA(0x8f,0x8f,0x8f,0xff));
    //blit layer layer little view
    max=mainFile->rect.w > mainFile->rect.h ? mainFile->rect.w : mainFile->rect.h;
    scale=max/25;

    member->navRect.w=mainFile->rect.w / scale;
    member->navRect.h=mainFile->rect.h / scale;
    member->navRect.x=30;
    member->navRect.y=(member->rect.h - member->navRect.h) / 2;

    member->navSurface=SDL_CreateRGBSurface(0,member->navRect.w,member->navRect.h,32,rmask,gmask,bmask,amask);


    SYS_DrawOpactiy_0(member->navSurface,NULL,4);

    SDL_BlitSurface(member->navSurface,NULL,member->surface,&member->navRect);
    SDL_BlitScaled(member->layer->surface,NULL,member->surface,&member->navRect);



    temp=TTF_RenderText_Blended(gFont,member->layer->name,white);

    member->textRect.x=member->navRect.x+ member->navRect.w +5;
    member->textRect.w= member->rect.w - 55 -member->textRect.x;
    member->textRect.w= member->textRect.w < temp->w ? member->textRect.w : temp->w;
    member->textRect.h=temp->h;
    member->textRect.y= (member->rect.h - member->textRect.h)/2;

    SDL_BlitSurface(temp,NULL,member->surface,&member->textRect);

    SDL_FreeSurface(temp);

    //temp=TTF_RenderText_Blended(gFont,member->layer->name,white);

    TTF_CloseFont(gFont);

    //lock icon
    temp=IMG_Load("icons\\lock.png");
    if(temp==NULL)
    {
        return 0;
    }
    if(!member->layer->lockPaint)
        SDL_SetSurfaceAlphaMod(temp,127);
    member->lockRect.x= member->rect.w-50;
    member->lockRect.w=temp->w;
    member->lockRect.h=temp->h;
    member->lockRect.y=(member->rect.h - member->lockRect.h)/2;

    SDL_BlitSurface(temp,NULL,member->surface,&member->lockRect);
    SDL_FreeSurface(temp);

    //alpha icon
    temp=IMG_Load("icons\\alpha.png");
    if(temp==NULL)
    {
        return 0;
    }
    if(!member->layer->lockAlpha)
        SDL_SetSurfaceAlphaMod(temp,127);
    member->AlphaRect.x= member->rect.w-25;
    member->AlphaRect.w=temp->w;
    member->AlphaRect.h=temp->h;
    member->AlphaRect.y=(member->rect.h - member->AlphaRect.h)/2;

    SDL_BlitSurface(temp,NULL,member->surface,&member->AlphaRect);
    SDL_FreeSurface(temp);

    /*
     *we will be continue later
     */

    //add member to the list
    if(list->member==NULL)
    {
        list->member=member;
    }
    else
    {
        currentMember=list->member;
        while(currentMember->next)
        {
            currentMember=currentMember->next;
        }
        currentMember->next=member;
    }

    return 1;
}

int DK_CreateLayerListMember_Surface(DK_LayerList *list)
{
    int i=0,hieght;
    DK_LayerListMember * member=list->member;

    while(member)
    {
        i++;
        member=member->next;
    }

    list->membersSurface=SDL_CreateRGBSurface(0,list->member->rect.w,list->member->rect.h * i,32,rmask,gmask,bmask,amask);
    if(list->membersSurface==NULL)
    {
        return 0;
    }

    //member surface rect
    list->memberRect.x=1;
    list->memberRect.y=1;
    list->memberRect.w=list->membersSurface->w;
    list->memberRect.h=list->membersSurface->h;

    hieght=list->memberRect.h;
    i=list->member->rect.h;

    member=list->member;
    while(member)
    {
        hieght-=i;
        member->rect.y=hieght;
        SDL_BlitSurface(member->surface,NULL,list->membersSurface,&member->rect);
        member=member->next;
    }

    return 1;
}

void DK_LayerListClicked(DK_LayerList *list, NV_Docker *docker, NV_Panel *panel,int x,int y)
{
    DR_MainViewport * NavPort= DR_FindViewport("navbar");
    DK_LayerListMember * member=list->member;
   // SDL_Rect rect;

    while(member && !SYS_InsideRect(x,y,&member->rect))
    {
        member=member->next;
    }
    if(member==NULL)
    {
puts("clicking on empty space");
        return;
    }

    //update location to specific list
    x-=member->rect.x;
    y-=member->rect.y;

    printf("%s clicked\n",member->layer->name);

    if(SYS_InsideRect(x,y,&member->eyeRect))
    {
        puts("visible eye click");
        member->layer->view=!member->layer->view;
        DF_UpdateFile();
    }
    else if(SYS_InsideRect(x,y,&member->navRect))
    {
        puts("nav view clicked");
    }
    else if(SYS_InsideRect(x,y,&member->textRect))
    {
        puts("layer name clicked");
    }
    else if(SYS_InsideRect(x,y,&member->lockRect))
    {
        puts("lock clicked");
        member->layer->lockPaint=!member->layer->lockPaint;
    }
    else if(SYS_InsideRect(x,y,&member->AlphaRect))
    {
        puts("alpha clicked");
        member->layer->lockAlpha=!member->layer->lockAlpha;
    }
    else
    {
        puts("just layer");
        mainFile->activeLayer=member->layer;
    }

    /*
    *
    *write later a funtcion just for update
    *
    */
    docker->UnsetDocker(docker);
    docker->InitDocker(docker);

    SDL_BlitSurface(docker->surface, NULL, panel->surface,& docker->rect);
    SDL_BlitSurface(panel->surface,NULL,NavPort->surface,&panel->rect);

}


