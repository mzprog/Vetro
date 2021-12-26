#include "toolOption.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../DR-vars.h"
#include "../core/sys.h"
#include "../DR-viewport.h"

TOOL_OptionSet * optionSetHead=NULL;

int TOOLOPTION_CreateOptionSet(const char * name, unsigned char type,void (*SetValue)(int))
{
    TOOL_OptionSet * currentToolOption=(TOOL_OptionSet *)malloc(sizeof(TOOL_OptionSet));
    TOOL_OptionSet * currentToolOption2=optionSetHead;
    if(currentToolOption==NULL)
    {
        return 0;
    }
    if(optionSetHead==NULL)
    {
        optionSetHead=currentToolOption;
    }
    else
    {

        while(currentToolOption2->next)
        {
            currentToolOption2=currentToolOption2->next;
        }
        currentToolOption2->next=currentToolOption;
    }

    strcpy(currentToolOption->name,name);
    currentToolOption->next=NULL;
    currentToolOption->typePointer.radio=NULL;
    currentToolOption->type=type;
    currentToolOption->SetValue=SetValue;
    return 1;
}

TOOL_OptionSet * TOOLOPTION_GetOptionSet(const char * name)
{
    TOOL_OptionSet * current=optionSetHead;
    while(current && strcasecmp(current->name,name))
    {
        current=current->next;
    }
    return current;
}

void TOOLOPTION_DestroyOptionBar()
{
    TOOL_OptionSet * optionSet,*optionSet2;
    DR_MainViewport * currentPort = DR_FindViewport("toolOption");
    optionSet=optionSetHead;

    while(optionSet)
    {
        optionSet2=optionSet->next;
        if(optionSet->type==TOOL_SET_RADIO)
        {
            TOOLOPTION_FreeRadioSet(optionSet->typePointer.radio);
        }
        SDL_FreeSurface(optionSet->surface);
        free(optionSet);

        optionSet=optionSet2;
    }
    optionSetHead=NULL;
    SDL_FillRect(currentPort->surface,NULL,currentPort->viewport->bgcolor);

}

void TOOLOPTION_FreeRadioSet(TOOL_RadioOption *radio)
{
    TOOL_RadioOption * current;

    while(radio)
    {
        current=radio->next;

        SDL_FreeSurface(radio->surface);

        free(radio);
        radio=current;
    }
}

int TOOLOPTION_CreateRadioOption(const char *setname, const char * radioname,const char * iconpath,unsigned char id, unsigned char select)
{
    TOOL_RadioOption * radioOption;
    TOOL_RadioOption * currentRadio=NULL;

    TOOL_OptionSet * optionset=TOOLOPTION_GetOptionSet(setname);
    if(optionset==NULL)
    {
        return 0;
    }
    radioOption= (TOOL_RadioOption *)malloc(sizeof(TOOL_RadioOption));
    if(radioOption==NULL)
    {
        return 0;
    }
    if(optionset->typePointer.radio==NULL)
    {
        optionset->typePointer.radio=radioOption;
    }
    else
    {
        currentRadio=optionset->typePointer.radio;
        while(currentRadio->next)
        {
            currentRadio=currentRadio->next;
        }
        currentRadio->next=radioOption;
    }

    strcpy(radioOption->name,radioname);
    strcpy(radioOption->icon,iconpath);
    radioOption->id=id;
    radioOption->select=select;
    radioOption->next=NULL;

    return 1;
}

int TOOLOPTION_CreateScrollBox(const char *setname, int value, int maxValue, const char * name, const char * unit,SDL_Rect * rect, int flag,void (*SetValue)(int))
{
    KIT_ScrollValueBox * box;

    TOOL_OptionSet * optionset=TOOLOPTION_GetOptionSet(setname);
    if(optionset==NULL)
    {
        return 0;
    }
    if(optionset->typePointer.sbox!=NULL)
    {
        return 0;
    }
    box=KIT_CreateScrollValueBox(value,maxValue,name,unit,rect,flag,SetValue);
    if(box==NULL)
    {
        return 0;
    }
    optionset->typePointer.sbox=box;

    return 1;

}

int TOOLOPTION_CreateOptionBar()
{
    DR_MainViewport * currentPort = DR_FindViewport("toolOption");
    int x=0;

    TOOL_OptionSet * currentSet=optionSetHead;

    while(currentSet)
    {
        if(currentSet->type==TOOL_SET_RADIO)
        {
            TOOLOPTION_CreateRadioSurface(currentSet);
        }
        else if(currentSet->type== TOOL_SET_SCROLLBOX)
        {
            TOOLOPTION_CreateScrollBoxSurface(currentSet);
        }

        currentSet->rect.x = x;
        currentSet->rect.y = 0;
        currentSet->rect.w = currentSet->surface->w;
        currentSet->rect.h = currentSet->surface->h;

        x += currentSet->surface->w;
        SDL_BlitSurface(currentSet->surface,NULL,currentPort->surface,&currentSet->rect);
        currentSet=currentSet->next;
    }
    return 1;
}

int TOOLOPTION_CreateRadioSurface(TOOL_OptionSet * port)
{
    TOOL_RadioOption * radioList=NULL;
    SDL_Rect rect;
    int i,j;

    if(port->type!=TOOL_SET_RADIO)
    {
        return 0;
    }
    rect.x=0;
    rect.y=0;
    rect.h=0;
    rect.w=5;
    radioList=port->typePointer.radio;
    while(radioList)
    {
        radioList->surface=IMG_Load(radioList->icon);
        if(radioList->surface==NULL)
        {
            return 0;
        }
        if(radioList->surface->h > rect.h)
        {
            rect.h=radioList->surface->h;
        }
        rect.w+=radioList->surface->w +2;
        //rect of the icon
        radioList->rect.y=0;
        radioList->rect.w=radioList->surface->w;
        radioList->rect.h=radioList->surface->h;

        radioList=radioList->next;
    }
    //nothing to do
    if(rect.w==5)
    {
        return 0;
    }
    rect.w+=5;//add 5 pixel of space after it
    port->surface=SDL_CreateRGBSurface(0,rect.w,rect.h,32,rmask,gmask,bmask,amask);
    if(port->surface==NULL)
    {
        return 0;
    }
    rect.x=5;

    radioList=port->typePointer.radio;
    while(radioList)
    {
        radioList->rect.x=rect.x;
        if(radioList->select)
        {
            SDL_FillRect(port->surface,&radioList->rect,SYS_RGBA(0x30,0x30,0x30,0xff));
        }
        SDL_BlitSurface(radioList->surface,NULL,port->surface,&radioList->rect);

        rect.x += radioList->surface->w +2;
        radioList=radioList->next;
    }
    j=port->surface->w - 1;
    for(i=0;i<port->surface->h;i++)
    {
        MZSDL_PutPixel32(port->surface,j,i,SYS_RGBA(0x18,0x18,0x18,0xff));
    }
    return 1;
}

int TOOLOPTION_CreateScrollBoxSurface(TOOL_OptionSet * port)
{
    SDL_Rect rect;
    int i,j;

    if(port==NULL)
    {
        return 0;
    }

    if(port->typePointer.sbox==NULL)
    {
        return 0;
    }

    rect.x=0;
    rect.y=0;
    rect.w=port->typePointer.sbox->rect.w +10;
    rect.h=port->typePointer.sbox->rect.h;

    port->surface=SDL_CreateRGBSurface(0,rect.w, rect.h, 32, rmask, gmask, bmask, amask);
    if(port->surface==NULL)
    {
        return 0;
    }

    port->typePointer.sbox->rect.x=5;
    port->typePointer.sbox->rect.y=0;

    SDL_BlitSurface(port->typePointer.sbox->surface,NULL,port->surface,&port->typePointer.sbox->rect);
    j=port->surface->w - 1;
    for(i=0;i<port->surface->h;i++)
    {
        MZSDL_PutPixel32(port->surface,j,i,SYS_RGBA(0x18,0x18,0x18,0xff));
    }
    return 1;
}

void TOOLOPTION_MouseEvent(int x,int y)
{
    DR_MainViewport * toolOptioPort=DR_FindViewport("toolOption");
    TOOL_OptionSet * optionSet=optionSetHead;

    if(toolOptioPort==NULL)
    {
        return;
    }

    x-=toolOptioPort->rect.x;
    y-=toolOptioPort->rect.y;
    //if option set clicked
    while(optionSet)
    {
        if(x >= optionSet->rect.x && x <= optionSet->rect.x + optionSet->rect.w)
        {
            if(y >= optionSet->rect.y && y <= optionSet->rect.y + optionSet->rect.h)
            {
                break;
            }
        }

        optionSet=optionSet->next;
    }

    if(optionSet==NULL)
    {
        return;//no set clicked
    }

    if(optionSet->type==TOOL_SET_RADIO)
    {
        x -= optionSet->rect.x;
        y -= optionSet->rect.y;
        TOOLOPTION_RadioEvent(optionSet,x,y);
    }
    else if(optionSet->type==TOOL_SET_SCROLLBOX)
    {
        TOOLOPTION_ScrollBoxEvent(optionSet,x,y);
    }

}

void TOOLOPTION_RadioEvent(TOOL_OptionSet * set, int x, int y)
{
    TOOL_RadioOption * radioSet=set->typePointer.radio;
    TOOL_RadioOption * radioSelect=NULL;
    while(radioSet)
    {
        if(x >= radioSet->rect.x && x <= radioSet->rect.x + radioSet->rect.w)
        {
            if(y >= radioSet->rect.y && y <= radioSet->rect.y + radioSet->rect.h)
            {
                break;
            }
        }

        radioSet=radioSet->next;
    }
    if(radioSet==NULL || radioSet->select)
    { //if no set clicked or a selected set have been clicked
        puts("return 2");
        return;
    }

    radioSelect=radioSet;

    radioSet=set->typePointer.radio;
    while(radioSet)
    {//turn off the prev selected option
        radioSet->select=0;
        radioSet=radioSet->next;
    }

    radioSelect->select=1;
    set->SetValue(radioSelect->id);

    TOOLOPTION_UpdateRadioSurface(set);
    TOOLOPTION_UpdateToolOption();
}




void TOOLOPTION_UpdateRadioSurface(TOOL_OptionSet * port)
{
    TOOL_RadioOption * radioList=NULL;
    int i,j;

    if(port->type!=TOOL_SET_RADIO)
    {
        return;
    }
    //erase old pixels
    SDL_FillRect(port->surface,NULL,0);
    //refill
    radioList=port->typePointer.radio;
    while(radioList)
    {
        if(radioList->select)
        {
            SDL_FillRect(port->surface,&radioList->rect,SYS_RGBA(0x30,0x30,0x30,0xff));
        }
        SDL_BlitSurface(radioList->surface,NULL,port->surface,&radioList->rect);

        radioList=radioList->next;
    }

    j=port->surface->w - 1;
    for(i=0;i<port->surface->h;i++)
    {
        MZSDL_PutPixel32(port->surface,j,i,SYS_RGBA(0x18,0x18,0x18,0xff));
    }
}

void TOOLOPTION_ScrollBoxEvent(TOOL_OptionSet * set, int x, int y)
{
    DR_MainViewport * toolOptioPort=DR_FindViewport("toolOption");
    int xBefore = toolOptioPort->rect.x + set->rect.x + set->typePointer.sbox->rect.x;
    int i;
    SDL_Event event;
    KIT_ScrollValueBox *box = set->typePointer.sbox;

    x -=(set->rect.x + box->rect.x);
    TOOLOPTION_UpdateScrollBox(set, x);
    box->SetValue(box->value);
    TOOLOPTION_UpdateToolOption();
    //update view
    DR_MainViewPort(renderer, &VP_Texture);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,BGTexture,NULL,NULL);
    SDL_RenderCopy(renderer,VP_Texture,NULL,NULL);

    //SDL_RenderCopy(renderer,tempTexture,&srect,&drect);
    if(GLOBAL_SELECTION_LINES)
    {
        SDL_RenderCopy(renderer,GLOBAL_SELECTION_LINES,&GLOBAL_SELECTION_LINES_SOURCE_Rect,&GLOBAL_SELECTION_LINES_DEST_Rect);
    }
    SDL_RenderPresent(renderer);

    while(SDL_WaitEvent(&event))
    {
        if(event.type==SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&i,NULL);
            i-=xBefore;

            TOOLOPTION_UpdateScrollBox(set, i);
            box->SetValue(box->value);
            TOOLOPTION_UpdateToolOption();
        }
        else if(event.type==SDL_MOUSEBUTTONUP)
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

}

void TOOLOPTION_UpdateScrollBox(TOOL_OptionSet * set, int x)
{
    //DR_MainViewport * toolOptioPort=DR_FindViewport("toolOption");
    KIT_ScrollValueBox *box = set->typePointer.sbox;

    SDL_Rect barRect;
    SDL_Rect textRect;
    TTF_Font * gFont;
    SDL_Color white={0xff,0xff,0xff};
    SDL_Surface * textSurface=NULL;
    char namechar[20];
    char valuechar[8];

    gFont = TTF_OpenFont( "segoeui.ttf", 12 );
    if(gFont==NULL)
    {
        return;
    }

    SDL_FillRect(box->surface,NULL,SYS_RGBA(0x7f,0x7f,0x7f,0xff));
    SYS_DrawVerticalLine(box->surface,0,0,box->rect.h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
    SYS_DrawVerticalLine(box->surface,box->rect.w-1,0,box->rect.h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));

    SYS_DrawHorizantalLine(box->surface,0,box->rect.w-1,0,SYS_RGBA(0x3f,0x3f,0x3f,0xff));
    SYS_DrawHorizantalLine(box->surface,0,box->rect.w-1,box->rect.h-1,SYS_RGBA(0x3f,0x3f,0x3f,0xff));

    barRect.x=1;
    barRect.y=1;
    //barRect.w=(rect->w-2)*value/maxValue;
    barRect.w=x;
    if(x<0)
    {
        barRect.w=0;
    }
    else if(x>box->rect.w-2)
    {
        barRect.w=box->rect.w-2;
    }
    barRect.h=box->rect.h-2;
    SDL_FillRect(box->surface,&barRect,SYS_RGBA(0x3f,0x5f,0x7f,0xff));

    //box->value=(box->maxValue*barRect.w)/(box->rect.w-2);
    if(box->flag==SCRL_LINEAR)
    {
        box->value=(box->maxValue*barRect.w)/(box->rect.w-2);
    }
    else if(box->flag==SCRL_SQRT)
    {
        box->value= box->maxValue * pow(barRect.w / (box->rect.w-2.0), 2);
    }


    itoa(box->value,valuechar,10);
    strcpy(namechar,box->name);
    strcat(namechar,": ");
    strcat(namechar,valuechar);
    strcat(namechar,box->unit);

    textSurface = TTF_RenderText_Blended(gFont,namechar,white);


    textRect.x=box->rect.w/2 - textSurface->w/2;
    textRect.y=box->rect.h/2 - textSurface->h/2;
    textRect.w=textSurface->w;
    textRect.h=textSurface->h;

    SDL_BlitSurface(textSurface,NULL,box->surface,&textRect);

    SDL_BlitSurface(box->surface,NULL,set->surface,&box->rect);

    SDL_FreeSurface(textSurface);
    TTF_CloseFont(gFont);
}

void TOOLOPTION_UpdateToolOption()
{
    DR_MainViewport * currentPort = DR_FindViewport("toolOption");

    TOOL_OptionSet * currentSet=optionSetHead;

    SDL_FillRect(currentPort->surface,NULL,currentPort->viewport->bgcolor);
    while(currentSet)
    {
        SDL_BlitSurface(currentSet->surface,NULL,currentPort->surface,&currentSet->rect);
        currentSet=currentSet->next;
    }
}
