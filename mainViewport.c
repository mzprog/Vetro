#include <stdlib.h>
#include <string.h>
#include "mainViewport.h"
#include "DR-viewport.h"
#include "core/sys.h"
#include "menubar/menu.h"
#include "menubar/submenu.h"
#include "DR-Viewport_event.h"
#include "menubar/menuevent.h"
#include "file/file.h"
#include "tools/tools.h"
#include "tools/toolbar.h"
#include "tools/toolOption.h"
#include "navigator/navigator.h"
#include "navigator/navigator-event.h"

void MAIN_CreateMainViewport()
{
    DR_Viewport * menuBar;
    DR_Viewport * SubMenu;
    DR_Viewport * bottomBar;
    DR_Viewport * navBar;
    DR_Viewport * toolBar;
    DR_Viewport * toolOption;
    DR_Viewport * drawPanel;


    menuBar=(DR_Viewport *) malloc(sizeof(DR_Viewport));
    if(menuBar==NULL)
    {
        printf("Error allocating the data/n");//we still need to do some edit
        return;
    }

    SubMenu=(DR_Viewport *) malloc(sizeof(DR_Viewport));
    if(SubMenu==NULL)
    {
        printf("Error allocating the data/n");//we still need to do some edit
        return;
    }

    bottomBar=(DR_Viewport *) malloc(sizeof(DR_Viewport));
    if(bottomBar==NULL)
    {
        printf("Error allocating the data/n");//we still need to do some edit
        return;
    }

    navBar=(DR_Viewport *) malloc(sizeof(DR_Viewport));
    if(navBar==NULL)
    {
        printf("Error allocating the data/n");//we still need to do some edit
        return;
    }
    toolBar=(DR_Viewport *) malloc(sizeof(DR_Viewport));
    if(toolBar==NULL)
    {
        printf("Error allocating the data/n");//we still need to do some edit
        return;
    }
    toolOption=(DR_Viewport *) malloc(sizeof(DR_Viewport));
    if(toolOption==NULL)
    {
        printf("Error allocating the data/n");//we still need to do some edit
        return;
    }
    drawPanel=(DR_Viewport *) malloc(sizeof(DR_Viewport));
    if(drawPanel==NULL)
    {
        printf("Error allocating the data/n");//we still need to do some edit
        return;
    }

    //menu bar data
    menuBar->hieght=19;
    menuBar->width=-1;//for all width
    menuBar->hieghtData=fixed;
    menuBar->widthData=fixed;
    menuBar->HAlign=undefined;
    menuBar->VAlign=top;
    menuBar->bgcolor=SYS_RGBA(0xff,0xff,0xff,0xff);

    //sub menu bar data
    SubMenu->hieght=40;
    SubMenu->width=-1;//for all width
    SubMenu->hieghtData=fixed;
    SubMenu->widthData=fixed;
    SubMenu->HAlign=undefined;
    SubMenu->VAlign=top;
    SubMenu->bgcolor=SYS_RGBA(0xaa,0xaa,0xaa,0xff);

    //the bottomBar data
    bottomBar->hieght=16;
    bottomBar->width=-1;//for all width
    bottomBar->hieghtData=fixed;
    bottomBar->widthData=fixed;
    bottomBar->HAlign=undefined;
    bottomBar->VAlign=bottom;
    bottomBar->bgcolor=SYS_RGBA(0,0xff,0xff,0xff);


     //tool bar data
    toolBar->hieght=25;
    toolBar->width=-1;//for all width
    toolBar->hieghtData=fixed;
    toolBar->widthData=fixed;
    toolBar->HAlign=undefined;
    toolBar->VAlign=top;
    toolBar->bgcolor=SYS_RGBA(0x62,0x62,0x62,0xff);


    //navigator data
    navBar->hieght=-1;
    navBar->width=250;//for all width
    navBar->hieghtData=fixed;
    navBar->widthData=fixed;
    navBar->HAlign=left;
    navBar->VAlign=undefined;
    navBar->bgcolor=SYS_RGBA(0x7f,0x7f,0x7f,0xff);

    //tool option data
    toolOption->hieght=30;
    toolOption->width=-1;//for all width
    toolOption->hieghtData=fixed;
    toolOption->widthData=fixed;
    toolOption->HAlign=undefined;
    toolOption->VAlign=top;
    toolOption->bgcolor=SYS_RGBA(0x50,0x50,0x50,0xff);

    //draw panel data
    drawPanel->hieght=-1;
    drawPanel->width=-1;//for all width
    drawPanel->hieghtData=fixed;
    drawPanel->widthData=fixed;
    drawPanel->HAlign=undefined;
    drawPanel->VAlign=undefined;
    drawPanel->bgcolor=SYS_RGBA(0x30,0x30,0x30,0xff);

    DR_InitViewport();
    DR_CreateViewPort(menuBar,"menu");
    DR_CreateViewPort(SubMenu,"submenu");
    DR_CreateViewPort(bottomBar,"bottom");
    DR_CreateViewPort(toolBar,"toolbar");
    DR_CreateViewPort(navBar,"navbar");
    DR_CreateViewPort(toolOption,"toolOption");
    DR_CreateViewPort(drawPanel,"drawp");

    MB_InitMenu();
}

void MAIN_ResizeViewPorts()
{
    DR_MainViewport * VP_Current=DR_GetViewportHead();

    DR_InitViewport();//update the unused rect

    while(VP_Current!=NULL)
    {
        DR_ResizeViewPort(VP_Current);

        VP_Current=VP_Current->next;
    }
    MB_UpdateMenuBar(MB_RESIZE);
    MB_CreateActiveSubMenuSurface();

    DR_UpdateToolBar();
    DF_CheckFileRect();
    DF_UpdateFile();
    NV_ResizeNavBar();
}


void MAIN_MouseCilck(int x,int y)
{
    DR_MainViewport * CurrentPort = DR_GetViewportHead();

    while(CurrentPort && !EV_ViewportClick(CurrentPort,x,y))
    {
        CurrentPort=CurrentPort->next;
    }
    if(CurrentPort && EV_ViewportClick(CurrentPort,x,y))
    {
        if(strcasecmp(CurrentPort->name,"menu")==0)
        {
            MB_MenuClicked(x,y);
        }
        else if(strcasecmp(CurrentPort->name,"submenu")==0)
        {
            MB_SubMenuClicked(x-CurrentPort->rect.x,y-CurrentPort->rect.y);
        }
        else if(strcasecmp(CurrentPort->name,"bottom")==0)
        {
puts("bottom");

        }
        else if(strcasecmp(CurrentPort->name,"navbar")==0)
        {
            NV_NavigatorMouseEvent(x, y);
        }
        else if(strcasecmp(CurrentPort->name,"drawp")==0)
        {
            DR_ExecuteEvent(x,y);
        }
        else if(strcasecmp(CurrentPort->name,"toolbar")==0)
        {
            DR_ToolBarClicked(x,y);
        }
        else if(strcasecmp(CurrentPort->name,"toolOption")==0)
        {
            TOOLOPTION_MouseEvent(x,y);
        }
    }
}

void MAIN_MouseMotion(int x,int y)
{

    DR_MainViewport * CurrentPort = DR_GetViewportHead();

    while(CurrentPort && !EV_ViewportClick(CurrentPort,x,y))
    {
        CurrentPort=CurrentPort->next;
    }
    if(CurrentPort && EV_ViewportClick(CurrentPort,x,y))
    {
        if(strcasecmp(CurrentPort->name,"menu")==0)
        {
            DR_SetArrowCursor();
        }
        else if(strcasecmp(CurrentPort->name,"submenu")==0)
        {
            DR_SetArrowCursor();
        }
        else if(strcasecmp(CurrentPort->name,"bottom")==0)
        {
            DR_SetArrowCursor();
        }
        else if(strcasecmp(CurrentPort->name,"navbar")==0)
        {
            DR_SetArrowCursor();
        }
        else if(strcasecmp(CurrentPort->name,"drawp")==0)
        {
            DR_SetToolCursor();
        }
        else if(strcasecmp(CurrentPort->name,"toolbar")==0)
        {
            DR_SetArrowCursor();
        }
        else if(strcasecmp(CurrentPort->name,"toolOption")==0)
        {
            DR_SetArrowCursor();
        }
    }
}
