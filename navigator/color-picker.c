#include "color-picker.h"

#include <SDL.h>

#include "../core/sys.h"
#include "../core/color.h"
#include "../core/gui-kit.h"
#include "color-wheel.h"
#include "color-wheel-event.h"

KIT_ScrollValueBox * redBox=NULL;
KIT_ScrollValueBox * greenBox=NULL;
KIT_ScrollValueBox * blueBox=NULL;

SDL_Rect redBoxRect;
SDL_Rect greenBoxRect;
SDL_Rect blueBoxRect;

SDL_Rect colorView;

DK_ColorWheel * colorWheel=NULL;
SDL_Rect colorWheelRect;

NV_Docker * ColorDocker=NULL;

int DK_CreateColorPicker(NV_Docker * d)
{
   d->surface=SDL_CreateRGBSurface(0,d->rect.w,d->rect.h,32,rmask,gmask,bmask,amask);
    if(d->surface==NULL)
    {
        return 0;
    }

    redBoxRect.x=10;
    redBoxRect.y=d->rect.h-20;;
    redBoxRect.w=(d->rect.w-40)/3;
    redBoxRect.h=15;

    greenBoxRect.x=redBoxRect.x + redBoxRect.w + 10;
    greenBoxRect.y=redBoxRect.y;
    greenBoxRect.w=redBoxRect.w;
    greenBoxRect.h=15;

    blueBoxRect.x=greenBoxRect.x + greenBoxRect.w + 10;
    blueBoxRect.y=redBoxRect.y;
    blueBoxRect.w=redBoxRect.w;
    blueBoxRect.h=15;

    colorView.x=10;
    colorView.y=redBoxRect.y-40;
    colorView.w=30;
    colorView.h=30;

    colorWheelRect.x=0;
    colorWheelRect.y=0;
    colorWheelRect.w=d->rect.w;
    colorWheelRect.h=colorView.y;



    redBox=KIT_CreateScrollValueBox(SYS_GetForegroundRedColor(),255,"Red","",&redBoxRect,SCRL_LINEAR,DK_SetRedValue);
    if(redBox==NULL)
    {
        return 0;
    }
    greenBox=KIT_CreateScrollValueBox(SYS_GetForegroundGreenColor(),255,"Green","",&greenBoxRect,SCRL_LINEAR,DK_SetGreenValue);
    if(greenBox==NULL)
    {
        return 0;
    }
    blueBox=KIT_CreateScrollValueBox(SYS_GetForegroundBlueColor(),255,"Blue","",&blueBoxRect,SCRL_LINEAR,DK_SetBlueValue);
    if(blueBox==NULL)
    {
        return 0;
    }

    SDL_FillRect(d->surface,NULL,SYS_RGBA(0x8f,0x8f,0x8f,0xff));

    colorWheel = DK_DrawColorWheel(&colorWheelRect);

    SDL_BlitSurface(redBox->surface,NULL,d->surface,&redBoxRect);
    SDL_BlitSurface(greenBox->surface,NULL,d->surface,&greenBoxRect);
    SDL_BlitSurface(blueBox->surface,NULL,d->surface,&blueBoxRect);
    SDL_BlitSurface(colorWheel->surface,NULL,d->surface,&colorWheel->rect);

    SDL_FillRect(d->surface,&colorView,SYS_GetForegroundColor());
    ColorDocker=d;


    return 1;
}

void DK_SetRedValue(int x)
{
    SYS_SetForegroundRedColor(x);
    SDL_FillRect(ColorDocker->surface,&colorView,SYS_GetForegroundColor());
    COLOR_UpdateHSL();
    DK_UpdateWheel(colorWheel);
    SDL_BlitSurface(colorWheel->surface,NULL,ColorDocker->surface,&colorWheel->rect);
}

void DK_SetGreenValue(int x)
{
    SYS_SetForegroundGreenColor(x);
    SDL_FillRect(ColorDocker->surface,&colorView,SYS_GetForegroundColor());
    COLOR_UpdateHSL();
    DK_UpdateWheel(colorWheel);
    SDL_BlitSurface(colorWheel->surface,NULL,ColorDocker->surface,&colorWheel->rect);
}


void DK_SetBlueValue(int x)
{
    SYS_SetForegroundBlueColor(x);
    SDL_FillRect(ColorDocker->surface,&colorView,SYS_GetForegroundColor());
    COLOR_UpdateHSL();
    DK_UpdateWheel(colorWheel);
    SDL_BlitSurface(colorWheel->surface,NULL,ColorDocker->surface,&colorWheel->rect);
}

void DK_UpdateRGBScrollBox()
{
    //updating
    KIT_ScrollValueBoxSetValue(redBox,SYS_GetForegroundRedColor());
    KIT_ScrollValueBoxSetValue(greenBox,SYS_GetForegroundGreenColor());
    KIT_ScrollValueBoxSetValue(blueBox,SYS_GetForegroundBlueColor());
    //copying
    SDL_BlitSurface(redBox->surface,NULL,ColorDocker->surface,&redBox->rect);
    SDL_BlitSurface(greenBox->surface,NULL,ColorDocker->surface,&greenBox->rect);
    SDL_BlitSurface(blueBox->surface,NULL,ColorDocker->surface,&blueBox->rect);
    SDL_FillRect(ColorDocker->surface,&colorView,SYS_GetForegroundColor());
}

void DK_UnsetColorPicker(NV_Docker * d)
{
    KIT_DestroyScrollValueBox(redBox);
    KIT_DestroyScrollValueBox(greenBox);
    KIT_DestroyScrollValueBox(blueBox);
}

void DK_UseColorPicker(NV_Docker * docker, NV_Panel * panel ,int x,int y)
{
    if(SYS_InsideRect(x - docker->rect.x,  y - docker->rect.y, &redBoxRect))
    {
        KIT_ScrollValueBoxEvent(redBox,docker, panel,x,y);
    }
    else if(SYS_InsideRect(x - docker->rect.x,  y - docker->rect.y, &greenBoxRect))
    {
        KIT_ScrollValueBoxEvent(greenBox,docker, panel,x,y);
    }
     else if(SYS_InsideRect(x - docker->rect.x,  y - docker->rect.y, &blueBoxRect))
    {
        KIT_ScrollValueBoxEvent(blueBox,docker, panel,x,y);
    }
    else if(SYS_InsideRect(x - docker->rect.x,  y - docker->rect.y, &colorWheel->rect))
    {
        DK_ColorWheelEvent(colorWheel,docker,panel,x,y);
    }

}
