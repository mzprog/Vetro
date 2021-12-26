#include "navigator.h"

#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "../core/sys.h"
#include "../DR-viewport.h"

#include "layers-dock.h"
#include "color-picker.h"

NV_Panel * PanelHead=NULL;
DR_MainViewport * NavPort = NULL;

//temp function
int tempInit(NV_Docker * d)
{
    d->surface=SDL_CreateRGBSurface(0,d->rect.w,d->rect.h,32,rmask,gmask,bmask,amask);
    if(d->surface==NULL)
    {
        return 0;
    }

    SDL_FillRect(d->surface,NULL,SYS_RGBA(0x8f,0x8f,0x8f,0xff));
    return 1;
}

void temUnset(NV_Docker * d)
{
    SDL_FreeSurface(d->surface);
}
void tempUse(NV_Docker * docker, NV_Panel * panel ,int x,int y)
{
   // void (*UseDocker)(struct NV_Docker *,NV_Panel *,int,int);
    printf("docker %s used at(%d,%d)\n",docker->name,x,y);
}


int DR_CreateNavigator()
{
    NV_Panel * currentPanel;
    NavPort=DR_FindViewport("navbar");

    currentPanel=NV_AddPanel();
    if(currentPanel==NULL)
    {
        return 0;
    }
    NV_AddDocker(currentPanel,"color picker",DK_CreateColorPicker,DK_UnsetColorPicker,DK_UseColorPicker);
    NV_AddDocker(currentPanel,"navigator",tempInit,temUnset,tempUse);

    currentPanel=NV_AddPanel();
    NV_AddDocker(currentPanel,"Layers",DK_CreateLayerDocker,temUnset,DK_UseLayerDocker);
    NV_AddDocker(currentPanel,"navigator",tempInit,temUnset,tempUse);

    //create the surface
    NV_CreateNavigatorSurface();

    return 1;
}

NV_Panel * NV_AddPanel()
{
    NV_Panel *current,*newPanel;

    newPanel=(NV_Panel*) malloc(sizeof(NV_Panel));
    if(newPanel==NULL)
    {
        return NULL;
    }
    //copying data
    newPanel->docker=NULL;
    newPanel->selectedDocker=NULL;
    newPanel->next=NULL;

    newPanel->headRect.x=0;
    newPanel->headRect.y=0;
    newPanel->headRect.w=NavPort->rect.w;
    newPanel->headRect.h=30;

    newPanel->rect.x=0;
    newPanel->rect.w=NavPort->rect.w;
    newPanel->rect.h=200;
    //align to the main data structure
    if(PanelHead==NULL)
    {
        PanelHead=newPanel;
        newPanel->rect.y=0;
    }
    else
    {
        current=PanelHead;
        while(current->next)
        {
            current=current->next;
        }
        current->next=newPanel;
        newPanel->rect.y=current->rect.y+current->rect.h;
    }
    return newPanel;
}

int NV_AddDocker(NV_Panel * panel,const char * name,int (*InitDocker)(struct NV_Docker *),void (*UnsetDocker)(struct NV_Docker *),void (*UseDocker)(struct NV_Docker *,NV_Panel *,int,int))
{
    NV_Docker * currentDocker;
    NV_Docker * newDocker=(NV_Docker *) malloc(sizeof(NV_Docker));
    if(newDocker==NULL)
    {
        return 0;
    }

    strcpy(newDocker->name,name);
    newDocker->next=NULL;
    newDocker->surface=NULL;

    newDocker->rect.x=2;
    newDocker->rect.y=panel->headRect.y+panel->headRect.h;
    newDocker->rect.w=panel->rect.w-4;
    newDocker->rect.h=panel->rect.h - newDocker->rect.y-2;

    newDocker->InitDocker=InitDocker;
    newDocker->UnsetDocker=UnsetDocker;
    newDocker->UseDocker=UseDocker;

    if(panel->docker==NULL)
    {
        panel->docker=newDocker;
        panel->selectedDocker=newDocker;
    }
    else
    {
        currentDocker=panel->docker;
        while(currentDocker->next)
        {
            currentDocker=currentDocker->next;
        }
        currentDocker->next=newDocker;
    }
    return 1;
}

int NV_CreateNavigatorSurface()
{
    NV_Panel *panel=PanelHead;
    //NV_Docker *dock;

    NV_PanelRectSize();

    while(panel)
    {
        //create surface
        panel->surface=SDL_CreateRGBSurface(0,panel->rect.w,panel->rect.h,32,rmask,gmask,bmask,amask);
        if(panel->surface==NULL)
        {
            return 0;
        }

        //create head
        NV_CreatePanelHead(panel);

        //init docker
        panel->selectedDocker->InitDocker(panel->selectedDocker);
        //blit surface
        SDL_BlitSurface(panel->selectedDocker->surface, NULL, panel->surface,& panel->selectedDocker->rect);
        SDL_BlitSurface(panel->surface,NULL,NavPort->surface,&panel->rect);

        //other panel
        panel=panel->next;
    }

    return 1;
}

int NV_CreatePanelHead(NV_Panel * panel)
{
    NV_Docker * docker;
    TTF_Font * gFont;
    SDL_Rect rect,rect2;
    SDL_Color white={0xff,0xff,0xff};

    gFont = TTF_OpenFont( "segoeui.ttf", 12 );

     if(gFont==NULL)
    {
        printf("Failed to load font\n");
        return 0;
    }
    //border 2px thickness
    SDL_FillRect(panel->surface,&panel->headRect,SYS_RGBA(0x5f,0x5f,0x5f,0xff));
    SYS_DrawHorizantalLine(panel->surface, panel->headRect.x, panel->headRect.w-1 +panel->headRect.x,panel->headRect.y,SYS_RGBA(0x2f,0x2f,0x2f,0xff));
    SYS_DrawHorizantalLine(panel->surface, panel->headRect.x, panel->headRect.w-1 +panel->headRect.x,panel->headRect.y+1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
    SYS_DrawHorizantalLine(panel->surface, panel->headRect.x, panel->headRect.w-1 +panel->headRect.x,panel->headRect.y + panel->headRect.h-2,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
    SYS_DrawHorizantalLine(panel->surface, panel->headRect.x, panel->headRect.w-1 +panel->headRect.x,panel->headRect.y + panel->headRect.h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));

    SYS_DrawVerticalLine(panel->surface, panel->rect.x,   0, panel->rect.h-1,SYS_RGBA(0x2f,0x2f,0x2f,0xff));
    SYS_DrawVerticalLine(panel->surface, panel->rect.x+1, 0, panel->rect.h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
    SYS_DrawVerticalLine(panel->surface, panel->rect.x+panel->rect.w-1, 0,panel->rect.h-1,SYS_RGBA(0x2f,0x2f,0x2f,0xff));
    SYS_DrawVerticalLine(panel->surface, panel->rect.x+panel->rect.w-2, 0,panel->rect.h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));

    SYS_DrawHorizantalLine(panel->surface, panel->rect.x, panel->rect.w-1 +panel->rect.x, panel->rect.h-1,SYS_RGBA(0x2f,0x2f,0x2f,0xff));
    SYS_DrawHorizantalLine(panel->surface, panel->rect.x, panel->rect.w-1 +panel->rect.x, panel->rect.h-2,SYS_RGBA(0x3f,0x3f,0x3f,0xff));

    //tabs
    docker=panel->docker;

    rect.x=panel->headRect.x+2;
    rect.y=panel->headRect.y+2;
    rect.w=panel->headRect.w-4;
    rect.h=panel->headRect.h-4;

    while(docker)
    {
        docker->headText= TTF_RenderText_Blended(gFont,docker->name,white);
        if(docker->headText==NULL)
        {
            return 0;
        }
        //tap rect data
        docker->headTextRect.x=rect.x+10;
        docker->headTextRect.y=rect.y+5;
        docker->headTextRect.w=docker->headText->w+10;
        docker->headTextRect.h=docker->headText->h+5;

        docker->headRect.x=rect.x;
        docker->headRect.y=rect.y;
        docker->headRect.w=docker->headTextRect.w+10;
        docker->headRect.h=docker->headTextRect.h+5;

        //docker->panelRect.x=panel->rect.x;
        //docker->panelRect.y=panel->rect.y;
        //docker->panelRect.w=panel->rect.w;
        //docker->panelRect.h=panel->rect.h;
        //remaining data
        rect.x+=docker->headTextRect.w+12;
        rect.w-=docker->headTextRect.w+7;

        docker=docker->next;
    }

     TTF_CloseFont(gFont);
    //for scroll head later
    docker=panel->docker;
    while(docker)
    {
        if(docker==panel->selectedDocker)
        {
            rect2.x=docker->headRect.x;
            rect2.y=docker->headRect.y;
            rect2.w=docker->headRect.w;
            rect2.h=docker->headRect.h+2;
            SDL_FillRect(panel->surface,&rect2,SYS_RGBA(0x8f,0x8f,0x8f,0xff));
        }
        SYS_DrawVerticalLine(panel->surface,docker->headTextRect.x+docker->headTextRect.w,panel->headRect.y+2,panel->headRect.y+panel->headRect.h-3,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
        SYS_DrawVerticalLine(panel->surface,docker->headTextRect.x+docker->headTextRect.w+1,panel->headRect.y+2,panel->headRect.y+panel->headRect.h-3,SYS_RGBA(0x3f,0x3f,0x3f,0xff));

        SDL_BlitSurface(docker->headText,NULL,panel->surface,&docker->headTextRect);
        docker=docker->next;
    }

    return 1;
}



NV_Panel * NV_GetPanelHead()
{
    return PanelHead;
}

NV_Panel * NV_GetPanelByDocker(NV_Docker * docker)
{
    NV_Panel *Ptmp=PanelHead;
    NV_Docker * Dtmp=NULL;
    while(Ptmp)
    {
        Dtmp= Ptmp->docker;
        while(Dtmp)
        {
            if(Dtmp==docker)
            {
                return Ptmp;
            }
            Dtmp=Dtmp->next;
        }

        Ptmp= Ptmp->next;
    }
    return NULL;
}

DR_MainViewport * NV_GetNavBarPort()
{
    return NavPort;
}

void NV_PanelRectSize()
{
    int portHieght=NavPort->rect.h;
    int panelCount=0;
    int panelY=0;
    int panelH;

    NV_Panel * panel=PanelHead;
    //count panels
    while(panel)
    {
        panelCount++;
        panel=panel->next;
    }
    if(panelCount==0)
    {
        return;
    }
    //equal height for every panel
    panelH=portHieght/panelCount;

    panel=PanelHead;
    while(panel)
    {
        panel->rect.h=panelH;
        panel->rect.y=panelY;
        panelY+=panelH;

        panel->selectedDocker->rect.h=panel->rect.h-panel->headRect.h-2;

        panel=panel->next;
    }

}

void NV_ResizeNavBar()
{
    NV_Panel *panel=PanelHead;

    NV_PanelRectSize();
    while(panel)
    {
        //create surface
        if(panel->surface->h!=panel->rect.h)
        {
            SDL_FreeSurface(panel->surface);
            panel->surface=SDL_CreateRGBSurface(0,panel->rect.w,panel->rect.h,32,rmask,gmask,bmask,amask);
            if(panel->surface==NULL)
            {
                return;
            }
            //create head
            NV_CreatePanelHead(panel);

            //init docker
            panel->selectedDocker->UnsetDocker(panel->selectedDocker);
            panel->selectedDocker->InitDocker(panel->selectedDocker);

        }
        //blit surface
        SDL_BlitSurface(panel->selectedDocker->surface, NULL, panel->surface,& panel->selectedDocker->rect);
        SDL_BlitSurface(panel->surface,NULL,NavPort->surface,&panel->rect);

        //other panel
        panel=panel->next;
    }
}
