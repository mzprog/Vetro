#ifndef _SUBMENU_H_
#define _SUBMENU_H_

#include "menu.h"
#include <SDL2/SDL.h>

void MB_InitSubMenu(MB_Menu *tab,const char * name,const char *logopath,int ID);
SDL_Surface * MB_SubMenuSurface(const char * name,const char *logopath);
void MB_InitSubRect(MB_Menu * tab);
void MB_CreateActiveSubMenuSurface();

void MB_SetSubViewport();



#endif // _SUBMENU_H_
