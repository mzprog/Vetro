#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "DR-viewport.h"
#include "DR-vars.h"
#include "core/sys.h"


//global varaible
DR_MainViewport * VP_Manager=NULL;
SDL_Rect UnusedRect;




void DR_InitViewport()
{
    //we still have the all screen
    UnusedRect.h=GLOBAL_SCREEN_HEIGHT;
    UnusedRect.w=GLOBAL_SCREEN_WIDTH;
    UnusedRect.x=0;
    UnusedRect.y=0;

}

void DR_CreateViewPort(DR_Viewport * newport,const char * name)
{
    SDL_Rect rect;
    if(newport->hieght!=-1 && newport->width !=-1)//if both are defined
    {

        return;
    }

    if(newport->hieght >0 && newport->width==-1 &&(newport->VAlign == top || newport->VAlign==bottom))
    {
        if(newport->hieght > UnusedRect.h)
        {
            puts("no enough area");
            return;
        }

        if(newport->VAlign==top)
        {
            rect.y=UnusedRect.y;
            UnusedRect.y+=newport->hieght;
            UnusedRect.h-=newport->hieght;

        }
        else if (newport->VAlign==bottom)
        {
            UnusedRect.h-=newport->hieght;
            rect.y=UnusedRect.y + UnusedRect.h;
        }
        //get the missing var
        //newport->width=UnusedRect.w;

        rect.h=newport->hieght;
        rect.w=UnusedRect.w;
        rect.x=UnusedRect.x;

    }
    else if(newport->width >0 && newport->hieght==-1 && (newport->HAlign == left || newport->HAlign==right))
    {
         if(newport->width > UnusedRect.w)
        {
            puts("no enough area");
            return;
        }

        if(newport->HAlign==left)
        {
            rect.x=UnusedRect.x;
            UnusedRect.x+=newport->width;
            UnusedRect.w-=newport->width;

        }
        else if (newport->HAlign==right)
        {
            UnusedRect.w-=newport->width;
            rect.x=UnusedRect.x + UnusedRect.w;
        }
        //get the missing var
       // newport->hieght=UnusedRect.h;

        rect.w=newport->width;

        rect.h=UnusedRect.h;
        rect.y=UnusedRect.y;
    }
    else if(newport->hieght==-1 && newport->width==-1)
    {
        rect.x=UnusedRect.x;
        rect.y=UnusedRect.y;
        rect.w=UnusedRect.w;
        rect.h=UnusedRect.h;

        UnusedRect.h=0;
        UnusedRect.w=0;
    }
    else
    {
        return;
    }
    DR_CreateViewPortInMain(newport,rect,name);
}

void DR_CreateViewPortInMain(DR_Viewport * newport,SDL_Rect rect,const char * name)
{
    DR_MainViewport * VP_Current=NULL;
    if(VP_Manager==NULL)
    {
        VP_Manager=(DR_MainViewport *) malloc(sizeof(DR_MainViewport));
        if(VP_Manager==NULL)
        {
            puts("the main viewport unable to created");
            return;
        }
         VP_Current=VP_Manager;

    }
    else//find the last node
    {

        VP_Current=VP_Manager;
        while(VP_Current->next)
        {
            VP_Current=VP_Current->next;
        }

         VP_Current->next=(DR_MainViewport *) malloc(sizeof(DR_MainViewport));
        if(VP_Current->next==NULL)
        {
            puts("the new viewport unable to created");
            return;
        }
        VP_Current=VP_Current->next;
    }



        VP_Current->next=NULL;
        VP_Current->viewport=newport;
        VP_Current->rect=rect;
        VP_Current->surface=SDL_CreateRGBSurface(0,rect.w,rect.h,32,rmask,gmask,bmask,amask);

        if(VP_Current->surface==NULL)
        {
            puts("Error creating surface of the viewport");
            printf("ERROR: %s\n",SDL_GetError());
            return;
        }
        SDL_FillRect(VP_Current->surface,NULL,VP_Current->viewport->bgcolor);
        strcpy(VP_Current->name,name);
}

void DR_MainViewPort(SDL_Renderer * ren, SDL_Texture ** Texture)
{
    DR_MainViewport * VP_Current= VP_Manager;
    SDL_Surface *surface=NULL;


    //allocate the surface
   surface=SDL_CreateRGBSurface(0,GLOBAL_SCREEN_WIDTH,GLOBAL_SCREEN_HEIGHT,32,rmask,gmask,bmask,amask);
    if(surface==NULL)
    {
        puts("unable to create Main surface");
        return;
    }



    while(VP_Current)
    {
        SDL_BlitSurface(VP_Current->surface,NULL,surface,&VP_Current->rect);

        VP_Current=VP_Current->next;
    }

    if(*Texture!=NULL)
    {
        SDL_DestroyTexture(*Texture);
    }

    *Texture= SDL_CreateTextureFromSurface(ren,surface);
    SDL_FreeSurface(surface);
}

void DR_ResizeViewPort(DR_MainViewport * port)
{
    if(port->viewport->hieght==-1)
    {
        port->rect.h=UnusedRect.h;
    }
    if(port->viewport->width==-1)
    {
        port->rect.w=UnusedRect.w;
    }
    if(port->viewport->HAlign==left)
    {
        port->rect.x=UnusedRect.x;
        UnusedRect.x+=port->viewport->width;
        UnusedRect.w-=port->viewport->width;
        port->rect.y=UnusedRect.y;
        port->rect.h=UnusedRect.h;
    }
    else if(port->viewport->HAlign==right)
    {
        UnusedRect.w-=port->viewport->width;
        port->rect.x=UnusedRect.x + UnusedRect.w;
        port->rect.y=UnusedRect.y;
        port->rect.h=UnusedRect.h;
    }
    if(port->viewport->VAlign==top)
    {
        port->rect.y=UnusedRect.y;
        UnusedRect.y+=port->viewport->hieght;
        UnusedRect.h-=port->viewport->hieght;
        port->rect.x=UnusedRect.x;
        port->rect.w=UnusedRect.w;
    }
    else if(port->viewport->VAlign==bottom)
    {
        UnusedRect.h-=port->viewport->hieght;
        port->rect.y=UnusedRect.y + UnusedRect.h;
        port->rect.x=UnusedRect.x;
        port->rect.w=UnusedRect.w;
    }
    DR_ResizeViewPortInMain(port);
}


void DR_ResizeViewPortInMain(DR_MainViewport * port)
{
    SDL_FreeSurface(port->surface);
    port->surface=SDL_CreateRGBSurface(0,port->rect.w,port->rect.h,32,rmask,gmask,bmask,amask);
    if(port->surface==NULL)
    {
        puts("Error creating surface of the viewport");
        printf("ERROR: %s\n",SDL_GetError());
        return;
    }
    SDL_FillRect(port->surface,NULL,port->viewport->bgcolor);
}

void DR_QuitViewPorts()
{
    DR_MainViewport *VP_Current=VP_Manager;
    DR_MainViewport *VP_prev=NULL;
    while(VP_Current)
    {
        //free elements
        free(VP_Current->viewport);
        SDL_FreeSurface(VP_Current->surface);
        //step forward and free last
        VP_prev=VP_Current;
        VP_Current=VP_Current->next;
        free(VP_prev);
    }
    VP_Manager=NULL;
}

DR_MainViewport * DR_FindViewport(const char * name)
{
    DR_MainViewport * Port = VP_Manager;

    while(Port && strcmp(Port->name,name))
    {
        Port=Port->next;
    }
    return Port;
}
DR_MainViewport * DR_GetViewportHead()
{
    return VP_Manager;
}
