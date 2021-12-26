#ifndef _TOOLBAR_H_
#define _TOOLBAR_H_

#include "tools.h"

int DR_CreateToolBar();
int DR_UpdateToolBar();

int DR_ToolBarClicked(int x,int y);
int DR_IconClicked(DR_Tools * tool, int x, int y);

#endif // _TOOLBAR_H_
