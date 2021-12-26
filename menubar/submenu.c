#include "submenu.h"
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "../core/sys.h"
#include "../DR-viewport.h"

DR_MainViewport * SubViewport=NULL;

void MB_InitSubMenu(MB_Menu *tab,const char * name,const char *logopath, int ID)
{
    MB_SubMenu * CurrentSub=NULL;
    MB_SubMenu * SubMenu=(MB_SubMenu *) malloc(sizeof(MB_SubMenu));

    if(SubViewport==NULL)
    {
        MB_SetSubViewport();
    }

    if(SubMenu==NULL)
    {
        return;
    }
    SubMenu->next=NULL;
    SubMenu->ID=ID;
    strcpy(SubMenu->value,name);
    SubMenu->surface=MB_SubMenuSurface(name,logopath);
    /**
        we will do rect after init all data
      **/
    if(tab->submenu==NULL)
    {
        tab->submenu=SubMenu;
    }
    else
    {
        CurrentSub=tab->submenu;
        while(CurrentSub->next)
        {
            CurrentSub=CurrentSub->next;
        }
        CurrentSub->next=SubMenu;
    }
}

SDL_Surface * MB_SubMenuSurface(const char * name,const char *logopath)
{
    SDL_Surface *image=NULL;
    SDL_Surface *text=NULL;
    SDL_Rect rect;
    SDL_Surface * element=NULL;
    TTF_Font * gFont;
    int h=0,w=0;
    SDL_Color black={0,0,0};

    //load image
    image=IMG_Load(logopath);
    if(!image)
    {
        printf("IMG_Load: %s\n", IMG_GetError());

        return NULL;
    }
    //load text
    gFont = TTF_OpenFont( "segoeui.ttf", 12 );

     if(gFont==NULL)
    {
        printf("Failed to load font\n");
    }
    text= TTF_RenderText_Blended(gFont,name,black);
    if(text==NULL)
    {
        return NULL;
    }

    w=(image->w>text->w?image->w:text->w)+14;
    h=image->h + text->h;

    element=SDL_CreateRGBSurface(0,w,h,32,rmask,gmask,bmask,amask);
    if(element==NULL)
    {
        printf("element error!\n");
        return NULL;
    }
    rect.y=0;
    rect.h=image->h;
    rect.x=element->w/2-image->w/2;
    rect.w=image->w;

    SDL_BlitSurface(image,NULL,element,&rect);

    rect.y=image->h;
    rect.h=text->h;
    rect.x=element->w/2-text->w/2;
    rect.w=text->w;

    SDL_BlitSurface(text,NULL,element,&rect);

    SDL_FreeSurface(image);
    SDL_FreeSurface(text);
    return element;
}


void MB_InitSubRect(MB_Menu * tab)
{
    int x=0;
    MB_SubMenu * SB_Current=tab->submenu;

    while(SB_Current)
    {
        SB_Current->rect.x=x;
        SB_Current->rect.y=SubViewport->rect.h-SB_Current->surface->h;
        SB_Current->rect.w=SB_Current->surface->w;
        SB_Current->rect.h=SB_Current->surface->h;

        x+=SB_Current->surface->w;

        SB_Current=SB_Current->next;
    }

}

void MB_CreateActiveSubMenuSurface()
{
    DR_MainViewport * currentViewport= SubViewport;
    MB_Menu * currentMenu = MB_MainMenuManager;
    MB_SubMenu * currentSub=NULL;
    int i,j;

    //find active menu
    while(currentMenu && !currentMenu->select)
    {
        currentMenu=currentMenu->next;
    }
    if(currentMenu==NULL)
    {
        return;
    }


    currentSub=currentMenu->submenu;
    while(currentSub)
    {
        SDL_BlitSurface(currentSub->surface,NULL,currentViewport->surface,&currentSub->rect);
        //draw separating line
        i=currentSub->rect.x+currentSub->rect.w;
        for(j=0;j<currentViewport->rect.h;j++)
        {
            MZSDL_PutPixel32( currentViewport->surface, i, j, SYS_RGBA(0xa0,0xa0,0xa0,0xff) );
        }

        currentSub=currentSub->next;

    }

}

void MB_SetSubViewport()
{
    DR_MainViewport * currentPort=DR_FindViewport("submenu");
    SubViewport=currentPort;
}
