#ifndef _SCROLLVALUEBOX_H_
#define _SCROLLVALUEBOX_H_

#include <SDL2/SDL.h>

#include "../../navigator/navigator.h"

#define SCRL_LINEAR 0
#define SCRL_SQRT   1

typedef struct
{
    int value;
    int maxValue;
    SDL_Rect rect;
    SDL_Surface *surface;
    //SDL_Surface *mainsurface;
    void (*SetValue)(int);
    char unit[4];
    char name[12];
    int flag;

} KIT_ScrollValueBox;

KIT_ScrollValueBox * KIT_CreateScrollValueBox(int value, int maxValue,const char * name, const char * unit, SDL_Rect * rect, int flag, void (*SetValue)(int));

void KIT_DestroyScrollValueBox(KIT_ScrollValueBox * box);

void KIT_ScrollValueBoxEvent(KIT_ScrollValueBox * box, NV_Docker *docker, NV_Panel * panel,int x,int y);

void KIT_ScrollValueBoxDrawEvent(KIT_ScrollValueBox * box, NV_Docker *docker, NV_Panel * panel,int x,int y);

void KIT_ScrollValueBoxSetValue(KIT_ScrollValueBox * box,int i);

#endif // _SCROLLVALUEBOX_H_
