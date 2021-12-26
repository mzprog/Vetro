#include "selectionTools.h"

#include "file.h"

int  SL_Intersection(SL_Point * p1, SL_Point * p2, SL_Point * p3, SL_Point * p4, SL_Point * ps)
{

  int dx1=p2->x-p1->x;
  int dx2=p4->x-p3->x;
  int dy1=p2->y-p1->y;
  int dy2=p4->y-p3->y;

  int n=dx2*dy1-dy2*dx1;
  if(n==0)
    return 0;

  double s=1.0*(dx1*(p3->y-p1->y)-dy1*(p3->x-p1->x))/(1.0*n);
  if ((s<0.0)||(s>1.0))
    return 0;

  double t=1.0*(dx2*(p3->y-p1->y)-dy2*(p3->x-p1->x))/(1.0*n);
  if((t<0.0)||(t>1.0))
    return 0;
  ps->x=(int)(dx1*t+p1->x);
  ps->y=(int)(dy1*t+p1->y);
  return 1;
}

int SL_PointInside(SL_Point * p)
{
    SL_Line * currentLine=DR_GetPolygonHead();

    SL_Point p1,p2,p3,p4,ps;
    int Inside=0;

    p1.x=-1 ;
    p1.y=p->y;
    p2.x=mainFile->w;
    p2.y=p->y;

    while(currentLine)
    {
        p3.x= currentLine->p1.x;
        p3.y= currentLine->p1.y;
        p4.x= currentLine->p2.x;
        p4.y= currentLine->p2.y;


        if(SL_Intersection(&p1,&p2,&p3,&p4,&ps))
            if (ps.x < p->x )
                Inside=!Inside;
    }
    return Inside;
}

int SL_FindFirstInterSection(int * x,SL_Point * p1,SL_Point * p2)
{
    int found=0;
    SL_Point p;
    SL_Line * currentLine=DR_GetPolygonHead();
    *x=p2->x;
    while(currentLine)
    {
        if(SL_Intersection(p1, p2, &currentLine->p1, &currentLine->p2, &p))
        {
            found=1;
            if(*x>p.x)
            {
                *x=p.x;
            }
        }
        currentLine=currentLine->next;
    }
    return found;
}

int SL_FindMinimumX()
{
    int min=mainFile->w;
    SL_Line * currentLine=DR_GetPolygonHead();

    while(currentLine)
    {
        if(currentLine->p1.x<min)
        {
            min=currentLine->p1.x;
        }
        if(currentLine->p2.x<min)
        {
            min=currentLine->p2.x;
        }
        currentLine=currentLine->next;
    }
    return min;
}


int SL_FindMinimumY()
{
    int min=mainFile->h;
    SL_Line * currentLine=DR_GetPolygonHead();

    while(currentLine)
    {
        if(currentLine->p1.y<min)
        {
            min=currentLine->p1.y;
        }
        if(currentLine->p2.y<min)
        {
            min=currentLine->p2.y;
        }
        currentLine=currentLine->next;
    }
    return min;
}


int SL_FindMaximumX()
{
    int max=0;
    SL_Line * currentLine=DR_GetPolygonHead();

    while(currentLine)
    {
        if(currentLine->p1.x>max)
        {
            max=currentLine->p1.x;
        }
        if(currentLine->p2.x>max)
        {
            max=currentLine->p2.x;
        }
        currentLine=currentLine->next;
    }
    return max;
}


int SL_FindMaximumY()
{
    int max=0;
    SL_Line * currentLine=DR_GetPolygonHead();

    while(currentLine)
    {
        if(currentLine->p1.y>max)
        {
            max=currentLine->p1.y;
        }
        if(currentLine->p2.y>max)
        {
            max=currentLine->p2.y;
        }
        currentLine=currentLine->next;
    }
    return max;
}
