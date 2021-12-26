#include "color-wheel-event.h"

#include <math.h>
#include <SDL.h>
#include <string.h>

#include "color-picker.h"
#include "color-wheel.h"
#include "../core/color.h"
#include "../DR-vars.h"
#include "../DR-viewport.h"
#include "../tools/brush.h"

void DK_ColorWheelEvent(DK_ColorWheel *whl, NV_Docker * docker, NV_Panel * panel, int x, int y)
{
    int Xbefore= panel->rect.x + docker->rect.x +whl->rect.x;
    int Ybefore= panel->rect.y + docker->rect.y +whl->rect.y;
    int centerX,centerY,raduis;
    double a13,a12;
    double c13,c12;
    double x0,x1;

    centerX = whl->rect.w/2;
    centerY = whl->rect.h/2;

    x = x - Xbefore;
    y = y - Ybefore;

    raduis = sqrt(pow(x - centerX,2) + pow(y - centerY,2));
    if(raduis<=centerX && raduis>=centerX - 20)
    {
        DK_ColorWheelHueEvent(whl,docker,panel,x,y);
    }
    else
    {
        if(y >= whl->p1Y && y <= whl->p2Y)
        {
            //line slopes
            a13 = ((whl->p1Y - whl->p3Y) * 1.0) / (whl->p1X - whl->p3X);
            a12 = ((whl->p1Y - whl->p2Y) * 1.0) / (whl->p1X - whl->p2X);

            //calculate c
            c13 = whl->p1Y - a13 * whl->p1X;
            c12 = whl->p1Y - a12 * whl->p1X;
            //calculate line width
            x0 = (y - c13) / a13;
            x1 = (y - c12) / a12;

            if(x >= x0 && x <= x1)
            {
                DK_ColorWheelTriangleEvent(whl,docker,panel,x,y);
            }
        }
    }

}

void DK_ColorWheelHueEvent(DK_ColorWheel *whl, NV_Docker * docker, NV_Panel * panel, int x, int y)
{
    SDL_Event event;
    int Xbefore= NV_GetNavBarPort()->rect.x + panel->rect.x + docker->rect.x +whl->rect.x;
    int Ybefore= NV_GetNavBarPort()->rect.y + panel->rect.y + docker->rect.y +whl->rect.y;



    DK_ColorWheelHueUpdate(whl,docker,panel,x,y);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,BGTexture,NULL,NULL);
    SDL_RenderCopy(renderer,VP_Texture,NULL,NULL);
    if(GLOBAL_SELECTION_LINES)
    {
        SDL_RenderCopy(renderer,GLOBAL_SELECTION_LINES,&GLOBAL_SELECTION_LINES_SOURCE_Rect,&GLOBAL_SELECTION_LINES_DEST_Rect);
    }
    SDL_RenderPresent(renderer);

    while(SDL_WaitEvent(&event))
    {
        if(event.type==SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&x,&y);
            x -= Xbefore;
            y -= Ybefore;
            DK_ColorWheelHueUpdate(whl,docker,panel,x,y);
        }
        else if(event.type == SDL_MOUSEBUTTONUP)
        {
            break;
        }

        DR_MainViewPort(renderer, &VP_Texture);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,BGTexture,NULL,NULL);
        SDL_RenderCopy(renderer,VP_Texture,NULL,NULL);
        if(GLOBAL_SELECTION_LINES)
        {
            SDL_RenderCopy(renderer,GLOBAL_SELECTION_LINES,&GLOBAL_SELECTION_LINES_SOURCE_Rect,&GLOBAL_SELECTION_LINES_DEST_Rect);
        }
        SDL_RenderPresent(renderer);
    }
    //check tool
    if(strcmp(DR_GetTool()->name,"brush")==0)
    {
        TOOL_UpdateBrushSurface(SYS_GetForegroundColor());
    }
}

void DK_ColorWheelHueUpdate(DK_ColorWheel *whl, NV_Docker * docker, NV_Panel * panel, double x, double y)
{
    double center,hue;
    double saturation,brightness;

    center=whl->rect.w/2;
    hue=270 - 180/M_PI * atan2(y-center,x-center);
    if(hue>360)
    {
         hue -=360;
    }
    hue=360- hue;

    brightness = SYS_GetForegroundBrightnessColor();
    saturation = SYS_GetForegroundSaturationColor();

    SYS_SetForegroundHSLColor(hue, saturation,brightness);
    COLOR_UpdateRGB();

    DK_UpdateWheel(whl);
    SDL_BlitSurface(whl->surface,NULL,docker->surface,&whl->rect);
    DK_UpdateRGBScrollBox();

    SDL_BlitSurface(docker->surface,NULL,panel->surface,&docker->rect);
    SDL_BlitSurface(panel->surface,NULL,NV_GetNavBarPort()->surface,&panel->rect);


}

void DK_ColorWheelTriangleEvent(DK_ColorWheel *whl, NV_Docker * docker, NV_Panel * panel, int x, int y)
{
    SDL_Event event;
    int Xbefore= NV_GetNavBarPort()->rect.x + panel->rect.x + docker->rect.x +whl->rect.x;
    int Ybefore= NV_GetNavBarPort()->rect.y + panel->rect.y + docker->rect.y +whl->rect.y;

    DK_ColorWheelTriangleUpdate(whl,docker,panel,x,y);//at clicking
    DR_MainViewPort(renderer, &VP_Texture);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,BGTexture,NULL,NULL);
    SDL_RenderCopy(renderer,VP_Texture,NULL,NULL);
    if(GLOBAL_SELECTION_LINES)
    {
        SDL_RenderCopy(renderer,GLOBAL_SELECTION_LINES,&GLOBAL_SELECTION_LINES_SOURCE_Rect,&GLOBAL_SELECTION_LINES_DEST_Rect);
    }
    SDL_RenderPresent(renderer);

    while(SDL_WaitEvent(&event))
    {
        if(event.type==SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&x,&y);
            x -= Xbefore;
            y -= Ybefore;
            DK_ColorWheelTriangleUpdate(whl,docker,panel,x,y);
        }
        else if(event.type == SDL_MOUSEBUTTONUP)
        {
            break;
        }

        DR_MainViewPort(renderer, &VP_Texture);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,BGTexture,NULL,NULL);
        SDL_RenderCopy(renderer,VP_Texture,NULL,NULL);
        if(GLOBAL_SELECTION_LINES)
        {
            SDL_RenderCopy(renderer,GLOBAL_SELECTION_LINES,&GLOBAL_SELECTION_LINES_SOURCE_Rect,&GLOBAL_SELECTION_LINES_DEST_Rect);
        }
        SDL_RenderPresent(renderer);
    }
    //check tool
    if(strcmp(DR_GetTool()->name,"brush")==0)
    {
        TOOL_UpdateBrushSurface(SYS_GetForegroundColor());
    }

}

void DK_ColorWheelTriangleUpdate(DK_ColorWheel *whl, NV_Docker * docker, NV_Panel * panel, double x, double y)
{

    int y0,y1,yDiff;
    double x0,x1,xDiff;
    double a13,a12;
    double c13,c12;
    double Hue,Brightness,Saturation;

    y0 = whl->p1Y;
    y1 = whl->p2Y;
    yDiff = y1 - y0;

    if(y<y0) y=y0;
    else if(y>y1) y=y1;
    //line slopes
    a13 = ((whl->p1Y - whl->p3Y) * 1.0) / (whl->p1X - whl->p3X);
    a12 = ((whl->p1Y - whl->p2Y) * 1.0) / (whl->p1X - whl->p2X);
    //calculate c
    c13 = whl->p1Y - a13 * whl->p1X;
    c12 = whl->p1Y - a12 * whl->p1X;
    //get hue
    //COLOR_UpdateHSL();
    Hue = SYS_GetForegroundHueColor();

    x0 = (y - c13) / a13;
    x1 = (y - c12) / a12;
    xDiff = x1 - x0;
    if(xDiff==0)xDiff=1;
    if(x<x0)x=x0;
    else if(x>x1)x=x1;
    //Brightness and brightness
    Brightness = (y - y0) * (100.0 /yDiff);
    Saturation = (x-x0) * 100/xDiff;
    SYS_SetForegroundHSLColor(Hue, Saturation,Brightness);
    COLOR_UpdateRGB();


    DK_UpdateWheel(whl);
    SDL_BlitSurface(whl->surface,NULL,docker->surface,&whl->rect);

    DK_UpdateRGBScrollBox();

    SDL_BlitSurface(docker->surface,NULL,panel->surface,&docker->rect);
    SDL_BlitSurface(panel->surface,NULL,NV_GetNavBarPort()->surface,&panel->rect);
}
