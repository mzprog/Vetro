#include "menuevent.h"

#include <SDL.h>
#include <string.h>
#include "menu.h"
#include "submenu.h"
#include "menuevent.h"
#include "../DR-vars.h"
#include "../file/selection.h"
#include "../file/selectionTasks.h"

void MB_MenuClicked(int x,int y)
{
    MB_Menu * current=MB_MainMenuManager;
    MB_Menu * current2;
    DR_MainViewport * currentViewport=DR_FindViewport("menu");

    while(current && !MB_TabClicked(current,x,y))
    {
        current=current->next;
    }
    if(current && MB_TabClicked(current,x,y))
    {
        if(current->select==0)
        {
            current2=MB_MainMenuManager;
            while(current2)
            {
                current2->select=0;
                current2=current2->next;
            }
            current->select=1;
            //update bar
            MB_UpdateMenuBar(MB_UPDATETAB);

            while(currentViewport && strcmp(currentViewport->name,"submenu"))
            {
                currentViewport=currentViewport->next;
            }
            SDL_FillRect(currentViewport->surface,NULL,currentViewport->viewport->bgcolor);
            MB_CreateActiveSubMenuSurface();

        }
    }

}
int MB_TabClicked(MB_Menu * tab,int x,int y)
{
    if(tab->rect.x<x && tab->rect.x+tab->rect.w>x)
   {
        if(tab->rect.y<y && tab->rect.y+tab->rect.h>y)
        {//MB_SubMenuSwitch
            return 1;
        }
   }
    return 0;

}

void MB_SubMenuClicked(int x,int y)
{
  //  DR_MainViewport * currentViewport= DR_FindViewport("submenu");
    MB_Menu * currentMenu = MB_MainMenuManager;
    MB_SubMenu * currentSub=NULL;



    //find active menu
    while(currentMenu && !currentMenu->select)
    {
        currentMenu=currentMenu->next;
    }
    if(currentMenu==NULL)
    {
        return ;
    }

     currentSub=currentMenu->submenu;

     while(currentSub && ! MB_SubTabClicked(currentSub,x,y))
     {
         currentSub=currentSub->next;
     }

     if(currentSub)
     {

printf("%s clicked \n",currentSub->value);
     }
     else
     {
         return;
     }

     MB_SubMenuSwitch(currentSub);
}

int MB_SubTabClicked(MB_SubMenu * tab,int x,int y)
{
    if(tab->rect.x<x && tab->rect.x+tab->rect.w>x)
   {
        if(tab->rect.y<y && tab->rect.y+tab->rect.h>y)
        {
            return 1;
        }
   }
    return 0;
}

void MB_SubMenuSwitch(MB_SubMenu * tab)
{
    switch (tab->ID)
    {
        case 5:
            GLOBAL_QUIT_PROGRAM=1;
            break;

        case 9:
            TOOL_AllSelect();
            DR_DrawSelectionArea();
            break;

        case 10:
            TOOL_Deselect();
            SDL_DestroyTexture(GLOBAL_SELECTION_LINES);
            GLOBAL_SELECTION_LINES=NULL;
            break;

        case 11:
            TOOL_InverseSelect();
            DR_DrawSelectionArea();
            break;
    }
}
