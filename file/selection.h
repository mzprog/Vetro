#ifndef _SELECTION_H_
#define _SELECTION_H_


typedef struct
{
    int x;
    int y;
} SL_Point;

typedef struct SL_Line
{
    SL_Point p1;
    SL_Point p2;

    struct SL_Line * next;
} SL_Line;

int DR_InitSelection();
int DR_GetSelectMode();
SL_Line * DR_GetPolygonHead();

void DR_SetSelectMode(int mode);

int DR_CreateSelection();
void DR_ClearLines();

int DR_AddSelectionLine(SL_Point *p1, SL_Point * p2);
int DR_DrawSelectionArea();


#endif // _SELECTION_H_
