#include "DR-Viewport_event.h"
#include "DR-Viewport.h"

int EV_ViewportClick(DR_MainViewport * port,int x,int y)
{
   if(port->rect.x<x && port->rect.x+port->rect.w>x)
   {
        if(port->rect.y<y && port->rect.y+port->rect.h>y)
        {
            return 1;
        }
   }
    return 0;

}
