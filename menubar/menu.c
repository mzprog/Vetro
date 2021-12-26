#include "menu.h"

#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "submenu.h"
#include "../core/sys.h"
#include "../DR-viewport.h"


MB_Menu * MB_MainMenuManager = NULL;
SDL_Surface * MB_Surface=NULL;

void MB_InitMenu()
{
    MB_InitFile();
    MB_InitEdit();
    MB_InitSelect();
    MB_InitView();
    MB_InitWindow();
    MB_InitHelp();


    MB_InitMenuSurface();
    MB_CreateActiveSubMenuSurface();
}

void MB_InitFile()
{

    MB_Menu * MB_Current=MB_InitMenuTab("File");

    MB_InitSubMenu(MB_Current,"New","newfile.png",1);
    MB_InitSubMenu(MB_Current,"Open","newfile.png",2);
    MB_InitSubMenu(MB_Current,"Save","newfile.png",3);
    MB_InitSubMenu(MB_Current,"Save as","newfile.png",4);
    MB_InitSubMenu(MB_Current,"Exit","newfile.png",5);
    MB_InitSubRect(MB_Current);

}
void MB_InitEdit()
{
    MB_Menu * MB_Current=MB_InitMenuTab("Edit");

    MB_InitSubMenu(MB_Current,"Undo","newfile.png",6);
    MB_InitSubMenu(MB_Current,"Redo","newfile.png",7);
    MB_InitSubMenu(MB_Current,"Setting","newfile.png",8);
    MB_InitSubRect(MB_Current);
}

void MB_InitSelect()
{
    MB_Menu * MB_Current=MB_InitMenuTab("Select");
    MB_InitSubMenu(MB_Current,"All","newfile.png",9);
    MB_InitSubMenu(MB_Current,"Deselect","newfile.png",10);
    MB_InitSubMenu(MB_Current,"Inverse","newfile.png",11);
    MB_InitSubRect(MB_Current);
}


void MB_InitView()
{
    MB_Menu * MB_Current=MB_InitMenuTab("Veiw");

    MB_InitSubMenu(MB_Current,"Tools","newfile.png",12);
    MB_InitSubMenu(MB_Current,"Values","newfile.png",13);
    MB_InitSubMenu(MB_Current,"Anything","newfile.png",14);
    MB_InitSubMenu(MB_Current,"Vector","newfile.png",15);
    MB_InitSubRect(MB_Current);
}
void MB_InitWindow()
{
    MB_Menu * MB_Current=MB_InitMenuTab("Window");

    MB_InitSubMenu(MB_Current,"Tools","newfile.png",16);
    MB_InitSubMenu(MB_Current,"Brushes","newfile.png",17);
    MB_InitSubMenu(MB_Current,"Layers","newfile.png",18);
    MB_InitSubMenu(MB_Current,"File1","newfile.png",19);
    MB_InitSubMenu(MB_Current,"File2","newfile.png",20);
    MB_InitSubRect(MB_Current);
}
void MB_InitHelp()
{
    MB_Menu * MB_Current=MB_InitMenuTab("Help");

    MB_InitSubMenu(MB_Current,"About","newfile.png",21);
    MB_InitSubMenu(MB_Current,"Tips","newfile.png",22);
    MB_InitSubMenu(MB_Current,"Tutorial","newfile.png",23);
    MB_InitSubRect(MB_Current);
}

MB_Menu * MB_InitMenuTab(const char * title)
{
    MB_Menu * current= (MB_Menu *) malloc(sizeof(MB_Menu));
    MB_Menu * current2=NULL;
    if(current==NULL)
    {
        printf("error \n");
        return NULL;
    }

    strcpy(current->value,title);

    current->select=0;
    current->submenu=NULL;
    current->next=NULL;

    if(MB_MainMenuManager==NULL)
    {
        current->select=1;
        MB_MainMenuManager=current;
    }
    else
    {
        current2=MB_MainMenuManager;
        while(current2->next)
        {
            current2=current2->next;
        }
        current2->next=current;
    }
    return current;
}

int MB_InitMenuSurface()
{
    SDL_Surface * textSurface=NULL;
    SDL_Rect rect,rect2;
    TTF_Font * gFont;

    DR_MainViewport * currentViewport= DR_FindViewport("menu");
    MB_Menu * currentMenu = MB_MainMenuManager;

    if(currentViewport==NULL)
    {
        return 0;
    }
    //copy the rect data
    rect.w=currentViewport->rect.w;
    rect.h=currentViewport->rect.h;
    rect.x=0;
    rect.y=3;

    gFont = TTF_OpenFont( "segoeui.ttf", 12 );

     if(gFont==NULL)
    {
        printf("Failed to load font\n");
        return 0;
    }

    MB_Surface= SDL_CreateRGBSurface(0,currentViewport->rect.w,currentViewport->rect.h,32,rmask,gmask,bmask,amask);
    if(MB_Surface==NULL)
    {
        return 0 ;
    }

    SDL_Color black={0,0,0};
    while(currentMenu && rect.w>0)
    {
        textSurface= TTF_RenderText_Blended(gFont,currentMenu->value,black);
        if(textSurface==NULL)
        {
            return 0 ;
        }

        //copy menu rect
        /*
        MAYBE BUG
        */
        currentMenu->rect.x=rect.x;
        currentMenu->rect.y=0;
        currentMenu->rect.w=textSurface->w+14;
        currentMenu->rect.h=19;

        //copy the rect2 data
        rect.x+=7;
        rect2.x=rect.x;
        rect2.y=rect.y;
        rect2.w=textSurface->w;
        rect2.h=textSurface->w;
        //edit rect data
        rect.x+=rect2.w+7;
        rect.w-=(rect2.w+7);
        SDL_BlitSurface(textSurface,NULL,MB_Surface,&rect2);


        SDL_FreeSurface(textSurface);

        currentMenu=currentMenu->next;
    }

    TTF_CloseFont(gFont);

    currentMenu=MB_MainMenuManager;

     while(currentMenu && !currentMenu->select)
     {
         currentMenu=currentMenu->next;
     }

     if(currentMenu &&currentMenu->select)
     {
         SDL_FillRect(currentViewport->surface,&currentMenu->rect,SYS_RGBA(0xd9,0xed,0xff,0xff));
     }

    rect2.x=0;
    rect2.y=0;
    rect2.w=MB_Surface->w;
    rect2.h=MB_Surface->h;
    SDL_BlitSurface(MB_Surface,NULL,currentViewport->surface,&rect2);

    return 1;

}


void MB_UpdateMenuBar(int flag)
{
    SDL_Rect rect;
    DR_MainViewport * currentViewport= DR_FindViewport("menu");
    MB_Menu * currentMenu = MB_MainMenuManager;

    rect.x=0;
    rect.y=0;
    rect.w=MB_Surface->w;
    rect.h=MB_Surface->h;

    //find selected tab
    currentMenu=MB_MainMenuManager;

    while(currentMenu && !currentMenu->select)
    {
     currentMenu=currentMenu->next;
    }


    if(flag==MB_RESIZE)
    {
        if(currentMenu &&currentMenu->select)
        {
         SDL_FillRect(currentViewport->surface,&currentMenu->rect,SYS_RGBA(0xd9,0xed,0xff,0xff));
        }
        rect.x=0;
        rect.y=0;
        rect.w=MB_Surface->w;
        rect.h=MB_Surface->h;

        SDL_BlitSurface(MB_Surface,NULL,currentViewport->surface,&rect);
    }
    else if (flag == MB_UPDATETAB)
    {
        SDL_FillRect(currentViewport->surface,NULL,currentViewport->viewport->bgcolor);
        if(currentMenu &&currentMenu->select)
        {
         SDL_FillRect(currentViewport->surface,&currentMenu->rect,SYS_RGBA(0xd9,0xed,0xff,0xff));
        }
        rect.x=0;
        rect.y=0;
        rect.w=MB_Surface->w;
        rect.h=MB_Surface->h;

        SDL_BlitSurface(MB_Surface,NULL,currentViewport->surface,&rect);
    }
}
