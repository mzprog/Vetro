#include "navigator-event.h"

#include "navigator.h"
#include "../DR-viewport.h"
#include "../core/sys.h"

void NV_NavigatorMouseEvent(int x, int y)
{
    DR_MainViewport * port=NV_GetNavBarPort();
    NV_Panel * panel=NV_GetPanelHead();
    x-=port->rect.x;
    y-=port->rect.y;

    while(panel && !SYS_InsideRect(x,y,&panel->rect))
    {

        panel=panel->next;
    }
    if(panel)
    {
        NV_PanelMouseEvent(x,y,panel);
    }
}

void NV_PanelMouseEvent(int x,int y,NV_Panel * panel)
{
    x-=panel->rect.x;
    y-=panel->rect.y;

    if(SYS_InsideRect(x,y,&panel->headRect))
    {
        NV_HeadPanelMouseEvent(x,y,panel);
    }
    else if(SYS_InsideRect(x,y,&panel->selectedDocker->rect))
    {
        NV_DockerMouseEvent(x,y,panel);
    }
}

void NV_HeadPanelMouseEvent(int x,int y,NV_Panel * panel)
{
    NV_Docker * docker=panel->docker;

    while(docker && !SYS_InsideRect(x,y,&docker->headRect))
    {
        docker=docker->next;
    }
    if(docker)
    {
        if(docker!=panel->selectedDocker)
        {

            NV_SwicthDocker(panel,docker);
        }
    }
}

void NV_DockerMouseEvent(int x,int y,NV_Panel * panel)
{
    panel->selectedDocker->UseDocker(panel->selectedDocker,panel,x,y);
}

void NV_SwicthDocker(NV_Panel * panel, NV_Docker * docker)
{
    DR_MainViewport * NavPort = NV_GetNavBarPort();
    panel->selectedDocker->UnsetDocker(panel->selectedDocker);

    docker->rect.x=panel->selectedDocker->rect.x;
    docker->rect.y=panel->selectedDocker->rect.y;
    docker->rect.w=panel->selectedDocker->rect.w;
    docker->rect.h=panel->selectedDocker->rect.h;

    panel->selectedDocker=docker;

    NV_CreatePanelHead(panel);
    panel->selectedDocker->InitDocker(panel->selectedDocker);

    SDL_BlitSurface(panel->selectedDocker->surface, NULL, panel->surface,& panel->selectedDocker->rect);
    SDL_BlitSurface(panel->surface,NULL,NavPort->surface,&panel->rect);
}
