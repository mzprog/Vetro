#ifndef _MENUEVENT_H_
#define _MENUEVENT_H_

#include <SDL2/SDL.h>
#include "../DR-viewport.h"
#include "menu.h"

void MB_MenuClicked(int x,int y);
int MB_TabClicked(MB_Menu * tab,int x,int y);

void MB_SubMenuClicked(int x,int y);
int MB_SubTabClicked(MB_SubMenu * tab,int x,int y);
void MB_SubMenuSwitch(MB_SubMenu * tab);

#endif // _MENUEVENT_H_
