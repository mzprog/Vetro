#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <SDL2/SDL.h>

typedef struct DR_Tools
{
    char name[32];
    SDL_Cursor * cursor;
    SDL_Surface * icon;
    SDL_Rect rect;
    int (*SetTool)(struct DR_Tools *);
    void (*UnSetTool)(struct DR_Tools *);
    void (*UseTool)(int,int);
    struct DR_Tools * next;
} DR_Tools;

int DR_InitTools();
DR_Tools * DR_InitTool();
void DR_SetTool(DR_Tools * tool);
DR_Tools * DR_GetTool();
DR_Tools * DR_GetToolHead();

void DR_SetToolCursor();
void DR_SetArrowCursor();

void DR_ExecuteEvent(int x, int y);

SDL_Texture * TOOL_CreateLineTexture(SDL_Surface * surface,SDL_Rect * srect, SDL_Rect * drect);
void TOOL_UpdateTexture(SDL_Rect * srect,SDL_Rect * drect);

#endif // _TOOLS_H_
