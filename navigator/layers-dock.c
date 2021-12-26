#include "layers-dock.h"

#include "../core/sys.h"
#include "../core/gui-kit.h"
#include "navigator.h"
#include "layer-list.h"
#include "tool-layer-option.h"

NV_Docker * mainDocker=NULL;//main docker pointer

KIT_ScrollValueBox * opacity=NULL;
SDL_Rect opacityRect;

KIT_SelectOption * blendMode= NULL;
SDL_Rect blendModeRect;

DK_LayerList * layerList=NULL;
SDL_Rect layerListRect;

DK_ToolLayerOption * layerOption=NULL;
SDL_Rect layerOptionRect;

int DK_CreateLayerDocker(NV_Docker * docker)
{
    char * blendModeOptions[]={"Normal","Darken","Multiply","Color Burn","Linear Burn","Lighten","Screen","Color Dodge","Linear Dodge","Overlay","Soft Light","Hard Light",
        "Vivid Light","Linear Light","Pin Light","Difference","Exclusion","Hue"};

    docker->surface=SDL_CreateRGBSurface(0,docker->rect.w,docker->rect.h,32,rmask,gmask,bmask,amask);

    if(docker->surface==NULL)
    {
        return 0;
    }

    mainDocker=docker;

    SDL_FillRect(docker->surface,NULL,SYS_RGBA(0x8f,0x8f,0x8f,0xff));

    //opacity box
    opacityRect.x=10;
    opacityRect.y=10;
    opacityRect.w=150;
    opacityRect.h=20;
    opacity = KIT_CreateScrollValueBox(100,100,"Opactiy","%",&opacityRect,SCRL_LINEAR,DK_OpacitySetValue);
    if(opacity==NULL)
    {
        return 0;
    }
    SDL_BlitSurface(opacity->surface,NULL,docker->surface,&opacityRect);

    //select blending mode box
    blendModeRect.x=10;
    blendModeRect.y=35;
    blendModeRect.w=150;
    blendModeRect.h=20;
    blendMode = KIT_CreateSelectOption(blendModeOptions,18,&blendModeRect);
    if(blendMode==NULL)
    {
        return 0;
    }
    SDL_BlitSurface(blendMode->surface,NULL,docker->surface,&blendModeRect);

    layerListRect.x=5;
    layerListRect.y=65;
    layerListRect.w=docker->rect.w-10;
    layerListRect.h=docker->rect.h- 80;

    layerList=DK_CreateLayerList(&layerListRect);
    if(layerList==NULL)
    {
        return 0;
    }
    SDL_BlitSurface(layerList->surface,NULL,docker->surface,&layerList->rect);

    layerOptionRect.x = 5;
    layerOptionRect.y = layerListRect.y + layerListRect.h;
    layerOptionRect.w=layerListRect.w;
    layerOptionRect.h=15;

    layerOption= DK_CreateToolLayerOption(&layerOptionRect);
    if(layerOption==NULL)
    {
        return 0;
    }
    SDL_BlitSurface(layerOption->surface,NULL,docker->surface,&layerOption->rect);

    return 1;
}

void DK_OpacitySetValue(int x)
{
    printf("new opicity = %d\n",x);
}

void DK_DestroyLayerDocker(NV_Docker *docker)
{

}

void DK_UseLayerDocker(NV_Docker * docker,NV_Panel * panel,int x,int y)
{
    if(SYS_InsideRect(x - docker->rect.x,  y - docker->rect.y, &opacityRect))
    {
puts("opacity box clicked");
        KIT_ScrollValueBoxEvent(opacity,docker, panel,x,y);
    }
    else if(SYS_InsideRect(x - docker->rect.x,  y - docker->rect.y,  &blendModeRect))
    {
puts("blend mode box clicked");
    }
    else if(SYS_InsideRect(x - docker->rect.x,  y - docker->rect.y,  &layerListRect))
    {
        DK_LayerListClicked(layerList,docker, panel,x-docker->rect.x -layerListRect.x, y - docker->rect.y - layerListRect.y);
    }
    else if(SYS_InsideRect(x - docker->rect.x,  y - docker->rect.y,  &layerOptionRect))
    {
        DK_LayerOptionClicked(layerOption, x-docker->rect.x -layerOptionRect.x, y - docker->rect.y - layerOptionRect.y);
        if(TLO_GetUpdate())
        {
            docker->UnsetDocker(docker);
            docker->InitDocker(docker);
            SDL_BlitSurface(docker->surface,NULL,panel->surface,&docker->rect);
            SDL_BlitSurface(panel->surface,NULL,NV_GetNavBarPort()->surface,&panel->rect);
            TLO_SetUpdate(0);
        }
    }
}

void DK_UpdateLayerDock()
{
    NV_Panel * panel=NV_GetPanelByDocker(mainDocker);
    DR_MainViewport * NavPort= DR_FindViewport("navbar");

    mainDocker->UnsetDocker(mainDocker);
    mainDocker->InitDocker(mainDocker);
    //puts("update");
    SDL_BlitSurface(mainDocker->surface, NULL, panel->surface,& mainDocker->rect);
    SDL_BlitSurface(panel->surface,NULL,NavPort->surface,&panel->rect);
}


