#ifndef _MENU_H_
#define _MENU_H_

#include <stdlib.h>
#include <SDL2/SDL.h>
#include "../mzsdl.h"
#include "../DR-viewport.h"

#define MB_RESIZE 1
#define MB_MOUSEOVER 2
#define MB_UPDATETAB 3

typedef struct MB_SubMenu{
    SDL_Rect rect;
    SDL_Surface * surface;
    char value[16];
    int ID;
    struct MB_SubMenu * next;
}MB_SubMenu;

typedef struct MB_Menu{
    char value[16];
    char select;//boolean variable
    SDL_Rect rect;
    MB_SubMenu * submenu;
    struct MB_Menu * next;
}MB_Menu;

extern MB_Menu * MB_MainMenuManager;

void MB_InitMenu();

void MB_InitFile();
void MB_InitEdit();
void MB_InitSelect();
void MB_InitView();
void MB_InitWindow();
void MB_InitHelp();


MB_Menu * MB_InitMenuTab(const char * title);


int MB_InitMenuSurface();

void MB_UpdateMenuBar(int flag);

#endif // _MENU_H_
