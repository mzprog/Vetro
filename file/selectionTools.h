#ifndef _SELECTIONTOOLS_H_
#define _SELECTIONTOOLS_H_

#include "selection.h"

int  SL_Intersection(SL_Point * p1, SL_Point * p2, SL_Point * p3, SL_Point * p4, SL_Point * ps);
int SL_PointInside(SL_Point * p);

int SL_FindFirstInterSection(int * x,SL_Point * p1,SL_Point * p2);

int SL_FindMinimumX();
int SL_FindMinimumY();
int SL_FindMaximumX();
int SL_FindMaximumY();

#endif // _SELECTIONTOOLS_H_
