#include <SDL.h>
#include "DR-event.h"
#include "DR-vars.h"
#include "DR-defs.h"
#include "mainViewport.h"

void  DR_EventHandler(SDL_Event * EventH)
{
    int x,y;

    if (EventH->type == SDL_QUIT)
    {
        GLOBAL_QUIT_PROGRAM = true;
    }
    if (EventH->type == SDL_WINDOWEVENT)
    {
        if(EventH->window.event == SDL_WINDOWEVENT_RESIZED)
        {
            GLOBAL_SCREEN_WIDTH=EventH->window.data1;
            GLOBAL_SCREEN_HEIGHT=EventH->window.data2;
            MAIN_ResizeViewPorts();
        }
    }
    else if(EventH->type == SDL_MOUSEMOTION)
    {
        SDL_GetMouseState(&x,&y);
        MAIN_MouseMotion(x,y);
    }
    else if(EventH->type == SDL_MOUSEBUTTONDOWN)
    {
        if(EventH->button.button == SDL_BUTTON_LEFT)
        {
            SDL_GetMouseState(&x,&y);
            MAIN_MouseCilck(x,y);
        }
    }
}
