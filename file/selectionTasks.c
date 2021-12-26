#include "selectionTasks.h"
#include "file.h"
#include "drawable.h"
#include "selection.h"

void TOOL_InverseSelect()
{
    int i,j;
    for(i=0;i<mainFile->w;i++)
    {
        for(j=0;j<mainFile->h;j++)
        {
            DR_SetDrawbleXY(i,j,255-DR_GetDrawableXY(i,j));
        }
    }
}

void TOOL_AllSelect()
{
    int i,j;

    DR_SetSelectMode(1);
    for(i=0;i<mainFile->w;i++)
    {
        for(j=0;j<mainFile->h;j++)
        {
            DR_SetDrawbleXY(i,j,255);
        }
    }
}

void TOOL_Deselect()
{
    int i,j;

    DR_SetSelectMode(0);
    for(i=0;i<mainFile->w;i++)
    {
        for(j=0;j<mainFile->h;j++)
        {
            DR_SetDrawbleXY(i,j,0);
        }
    }
}
