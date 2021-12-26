#include "tool-layer-option.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>

#include "../core/sys.h"
#include "../file/layer.h"

char TLO_Update=0;

void tempTask()
{
    printf("task here");
}

DK_ToolLayerOption * DK_CreateToolLayerOption(SDL_Rect * rect)
{
    int x=2,i;
    DK_ToolLayerOptionMember * member;
    DK_ToolLayerOption * tool = (DK_ToolLayerOption*) malloc(sizeof(DK_ToolLayerOption));

    char * icons[]={"icons\\add-layer.png","icons\\group-layer.png","icons\\delete-layer.png"};
    void (*task[])()={DK_OptionAddLayer,tempTask,DK_OptionDeleteLayer};

    if(tool==NULL)
    {
        return NULL;
    }

    tool->surface = SDL_CreateRGBSurface(0,rect->w,rect->h,32,rmask,gmask,bmask,amask);
    if(tool->surface==NULL)
    {
        free(tool);
        return NULL;
    }

    SDL_FillRect(tool->surface,NULL,SYS_RGBA(0x5f, 0x5f, 0x5f, 0xff));

    tool->member=NULL;

    tool->rect.x=rect->x;
    tool->rect.y=rect->y;
    tool->rect.w=rect->w;
    tool->rect.h=rect->h;

    for(i=0;i<3;i++)
    {
        member=DK_AddToolLayerOptionMemeber(tool,icons[i],task[i]);
        if(member==NULL)
        {
            return NULL;
        }
        member->rect.x=x;
        x = x + member->rect.w +4;

        SDL_BlitSurface(member->surface,NULL,tool->surface,&member->rect);
    }

    return tool;

}

DK_ToolLayerOptionMember * DK_AddToolLayerOptionMemeber(DK_ToolLayerOption * tool, const char * path, void (*task)())
{
    DK_ToolLayerOptionMember * current = tool->member;
    DK_ToolLayerOptionMember * member  = (DK_ToolLayerOptionMember *) malloc(sizeof(DK_ToolLayerOptionMember));
    if(member==NULL)
    {
        return 0;
    }

    member->surface = IMG_Load(path);
    if(member->surface==NULL)
    {
        return NULL;
    }
    member->next=NULL;
    member->task=task;

    member->rect.y=0;
    member->rect.w=member->surface->w;
    member->rect.h=member->surface->h;

    if(tool->member==NULL)
    {
        tool->member=member;
    }
    else
    {
        while(current->next)
        {
            current=current->next;
        }
        current->next=member;
    }
    return member;
}

void DK_LayerOptionClicked(DK_ToolLayerOption * tool, int x, int y)
{
    DK_ToolLayerOptionMember * member = tool->member;
    while(member)
    {
        if(SYS_InsideRect(x,y,&member->rect))
        {
            member->task();
            break;
        }
        member=member->next;
    }
}

void DK_OptionAddLayer()
{
    DF_CreateLayer(mainFile,0);
    TLO_SetUpdate(1);
}

void DK_OptionDeleteLayer()
{
    DF_DeleteLayer(mainFile);
    DF_UpdateFile();
    TLO_SetUpdate(1);
}

void TLO_SetUpdate(char a)
{
    TLO_Update=a;
}

char TLO_GetUpdate()
{
    return TLO_Update;
}
