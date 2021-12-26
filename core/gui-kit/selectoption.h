#ifndef _SELECTOPTION_H_
#define _SELECTOPTION_H_

#include <SDL2/SDL.h>

typedef struct KIT_Option
{
    char name[16];
    SDL_Rect rect;
    SDL_Rect textRect;
    SDL_Surface * surface;
    int ID;

    struct KIT_Option * next;
} KIT_Option;

typedef struct
{
    SDL_Rect rect;
    SDL_Rect listRect;
    SDL_Surface *surface;
    SDL_Surface *listSurface;

    KIT_Option * option;
    KIT_Option * selectedOption;

} KIT_SelectOption;

KIT_SelectOption * KIT_CreateSelectOption(char ** options,int size ,SDL_Rect * rect);

int KIT_AddOption(KIT_SelectOption *box, const char * option);

int KIT_GetMaxOptionWidth(KIT_SelectOption *box);

#endif // _SELECTOPTION_H_
