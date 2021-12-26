#ifndef _TOOL_LAYER_OPTION_H_
#define _TOOL_LAYER_OPTION_H_

#include <SDL2/SDL.h>

typedef struct DK_ToolLayerOptionMember
{
    SDL_Surface * surface;
    SDL_Rect rect;

    void (*task)();

    struct DK_ToolLayerOptionMember *next;
}DK_ToolLayerOptionMember;


typedef struct{

    DK_ToolLayerOptionMember * member;

    SDL_Surface * surface;
    SDL_Rect rect;

} DK_ToolLayerOption ;

DK_ToolLayerOption * DK_CreateToolLayerOption(SDL_Rect * rect);
DK_ToolLayerOptionMember * DK_AddToolLayerOptionMemeber(DK_ToolLayerOption * tool, const char * path, void (*task)());

void DK_LayerOptionClicked(DK_ToolLayerOption * tool, int x, int y);

void DK_OptionAddLayer();
void DK_OptionDeleteLayer();

void TLO_SetUpdate(char a);
char TLO_GetUpdate();

#endif // _TOOL_LAYER_OPTION_H_
