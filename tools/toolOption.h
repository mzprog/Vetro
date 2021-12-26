#ifndef _TOOLOPTION_H_
#define _TOOLOPTION_H_

#include <SDL2/SDL.h>
#include "../core/gui-kit/scrollvaluebox.h"

#define TOOL_SET_RADIO 1
#define TOOL_SET_SCROLLBOX 2

typedef struct TOOL_RadioOption
{
    char name[16];
    char icon[32];
    unsigned char id;
    unsigned char select;
    SDL_Surface * surface;
    SDL_Rect rect;
    struct TOOL_RadioOption * next;

} TOOL_RadioOption;

typedef union TOOL_OptionType
{
    TOOL_RadioOption * radio;
    KIT_ScrollValueBox * sbox;
} TOOL_OptionType;

typedef struct TOOL_OptionSet
{
    char name[16];
    unsigned char type;
    unsigned char select;

    TOOL_OptionType typePointer;

    SDL_Surface *surface;
    SDL_Rect rect;
    /* adding variable for complete process if we need it */
    void (*SetValue)(int);
    struct TOOL_OptionSet * next;

} TOOL_OptionSet;

int TOOLOPTION_CreateOptionSet(const char * name, unsigned char type,void (*SetValue)(int));
TOOL_OptionSet * TOOLOPTION_GetOptionSet(const char * name);

void TOOLOPTION_DestroyOptionBar();
void TOOLOPTION_FreeRadioSet(TOOL_RadioOption *radio);

int TOOLOPTION_CreateRadioOption(const char *setname, const char * radioname,const char * iconpath,unsigned char id, unsigned char select);
int TOOLOPTION_CreateRadioSurface(TOOL_OptionSet * port);

int TOOLOPTION_CreateScrollBox(const char *setname, int value, int maxValue, const char * name, const char * unit,SDL_Rect * rect, int flag,void (*SetValue)(int));
int TOOLOPTION_CreateScrollBoxSurface(TOOL_OptionSet * port);

int TOOLOPTION_CreateOptionBar();

void TOOLOPTION_MouseEvent(int x,int y);
void TOOLOPTION_RadioEvent(TOOL_OptionSet * set, int x, int y);

void TOOLOPTION_UpdateRadioSurface(TOOL_OptionSet * port);
void TOOLOPTION_UpdateToolOption();

void TOOLOPTION_ScrollBoxEvent(TOOL_OptionSet * set, int x, int y);
void TOOLOPTION_UpdateScrollBox(TOOL_OptionSet * set, int x);

#endif // _TOOLOPTION_H_
