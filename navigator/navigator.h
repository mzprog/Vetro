#ifndef _NAVIGATOR_H_
#define _NAVIGATOR_H_

#include <SDL2/SDL.h>

#include "../DR-viewport.h"

typedef struct NV_Panel NV_Panel;

typedef struct NV_Docker{
    char name[16];
    SDL_Rect rect;
    SDL_Rect headRect;
    //SDL_Rect panelRect;
    SDL_Rect headTextRect;
    SDL_Surface * surface;
    SDL_Surface * headText;

    //char selected;

    int (*InitDocker)(struct NV_Docker *);
    void (*UnsetDocker)(struct NV_Docker *);
    void (*UseDocker)(struct NV_Docker *,NV_Panel *,int,int);

    struct NV_Docker *next;
} NV_Docker;

typedef struct NV_Panel{
    SDL_Rect rect;
    SDL_Rect headRect;
    SDL_Surface *surface;

    NV_Docker * docker;
    NV_Docker * selectedDocker;

    struct NV_Panel *next;

} NV_Panel;

int DR_CreateNavigator();
NV_Panel * NV_AddPanel();
int NV_AddDocker(NV_Panel * panel,const char * name,int (*InitDocker)(struct NV_Docker *),void (*UnsetDocker)(struct NV_Docker *),void (*UseDocker)(struct NV_Docker *,NV_Panel *,int,int));
int NV_CreateNavigatorSurface();
int NV_CreatePanelHead(NV_Panel * panel);

NV_Panel * NV_GetPanelHead();
NV_Panel * NV_GetPanelByDocker(NV_Docker * docker);
DR_MainViewport * NV_GetNavBarPort();
void NV_PanelRectSize();
void NV_ResizeNavBar();

#endif // _NAVIGATOR_H_
