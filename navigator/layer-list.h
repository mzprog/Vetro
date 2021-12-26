#ifndef _LAYER_LIST_H_
#define _LAYER_LIST_H_

#include <SDL2/SDL.h>

#include "../file/file.h"
#include "navigator.h"

typedef struct DK_LayerListMember
{
    SDL_Surface * surface;
    SDL_Rect rect;

    SDL_Rect eyeRect;
    SDL_Rect lockRect;
    SDL_Rect AlphaRect;
    SDL_Rect navRect;
    SDL_Rect textRect;

    SDL_Surface *navSurface;

    DF_Layer *layer;

    struct DK_LayerListMember *next;
} DK_LayerListMember;

typedef struct
{
    SDL_Surface *surface;
    SDL_Rect rect;

    DK_LayerListMember * member;

    SDL_Surface *membersSurface;
    SDL_Rect memberRect;

} DK_LayerList;

DK_LayerList * DK_CreateLayerList(SDL_Rect *rect);
int DK_CreateLayerListMember(DK_LayerList *list,SDL_Rect *rect ,int width);
int DK_CreateLayerListMember_Single(DK_LayerList *list, DF_Layer *layer, int width);
int DK_CreateLayerListMember_Surface(DK_LayerList *list);

void DK_LayerListClicked(DK_LayerList *list, NV_Docker *docker, NV_Panel *panel,int x,int y);


#endif // _LAYER_LIST_H_
