#include "brush.h"

#include <SDL.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../core/sys.h"
#include "../core/color.h"
#include "tools.h"
#include "../file/drawable.h"
#include "../file/file.h"
#include "../DR-viewport.h"
#include "line-plot.h"
#include "../DR-vars.h"
#include "toolOption.h"
#include "../navigator/layers-dock.h"

#define BRUSH_MODE 0
#define ERASER_MODE 1

int size=20;
float space=100;
int TOOL_Mode = BRUSH_MODE;

int THREAD_ON_BRUSH=0;
SDL_SpinLock THREADONBRUSH_lock = 0;

int RENDERUPDATE = 0;
SDL_SpinLock RENDERUPDATE_lock = 0;

TOOL_BrushPoint * BrushPointHead = NULL;
SDL_SpinLock BrushPointHead_lock = 0;

//temp data
SDL_Texture * tempTexture=NULL;
SDL_Surface * tempSurface=NULL;

int Xcenter,Ycenter;
SDL_Surface * BrushSurface=NULL;


int TOOL_SetBrush(DR_Tools * brush)
{
    SDL_Rect boxRect;
    SDL_Rect SpaceRect;

    SDL_Surface * surface=SDL_CreateRGBSurface(0,size*2+2,size*2+2,32,rmask,gmask,bmask,amask);
    if(surface==NULL)
    {
        return 0;
    }
    BrushSurface=SDL_CreateRGBSurface(0,size*2+2,size*2+2,32,rmask,gmask,bmask,amask);
    Xcenter=size+1;
    Ycenter=size+1;
    TOOL_DrawCircle(surface,size,Xcenter,Ycenter);
    brush->cursor=SDL_CreateColorCursor(surface,Xcenter,Ycenter);
    strcpy(brush->name,"brush");

    boxRect.x=5;
    boxRect.y=0;
    boxRect.w=150;
    boxRect.h=30;

    SpaceRect.x=160;
    SpaceRect.y=0;
    SpaceRect.w=150;
    SpaceRect.h=30;

    TOOLOPTION_CreateOptionSet("size",TOOL_SET_SCROLLBOX,TOOL_BrushSetValueSize);
    TOOLOPTION_CreateScrollBox("size",size,1000,"Size","px",&boxRect,SCRL_SQRT,TOOL_BrushBoxSetValueSize);

    TOOLOPTION_CreateOptionSet("space",TOOL_SET_SCROLLBOX,TOOL_BrushSetValueSpace);
    TOOLOPTION_CreateScrollBox("space",space,1000,"Space","%",&SpaceRect,SCRL_SQRT,TOOL_BrushBoxSetValueSpace);

    TOOLOPTION_CreateOptionSet("Mode",TOOL_SET_RADIO,TOOL_SetValueMode);
    TOOLOPTION_CreateRadioOption("Mode","Brush","icons\\brush-radio.png",BRUSH_MODE,TOOL_Mode==BRUSH_MODE);
    TOOLOPTION_CreateRadioOption("Mode","Erazer","icons\\eraser.png",ERASER_MODE,TOOL_Mode==ERASER_MODE);

    TOOLOPTION_CreateOptionBar();

    return 1;
}


void TOOL_DrawCircle(SDL_Surface * surface, int raduis, int centerX, int centerY)
{

    double r2=raduis*raduis;
    double x,y=raduis;
    Uint32 alpha,alpha2;
    int w=raduis/sqrt(2) +1;//*18/24;
    Uint32 color=SYS_GetForegroundColor() & ~amask;

    for(x=0;x<w;x+=y/raduis)
    {
        y=sqrt(r2-x*x);

        alpha2=255*LINE_RFPart(y);
        if(alpha2<127)
        {
            alpha2/=2;
            alpha2=255-alpha2;
        }
        alpha2<<=ashift;

        MZSDL_PutPixel32(surface,centerX+x,centerY+y,amask);
        MZSDL_PutPixel32(surface,centerX+x,centerY-y,amask);
        MZSDL_PutPixel32(surface,centerX-x,centerY+y,amask);
        MZSDL_PutPixel32(surface,centerX-x,centerY-y,amask);

        MZSDL_PutPixel32(surface,centerY+y,centerX+x,amask);
        MZSDL_PutPixel32(surface,centerY-y,centerX+x,amask);
        MZSDL_PutPixel32(surface,centerY+y,centerX-x,amask);
        MZSDL_PutPixel32(surface,centerY-y,centerX-x,amask);

        //brush
        MZSDL_PutPixel32(BrushSurface,centerX+x,centerY+y,color|alpha2);
        MZSDL_PutPixel32(BrushSurface,centerX-x,centerY+y,color|alpha2);
        TOOL_FillLine(BrushSurface,centerX-x+1,centerY+y,centerX+x-1);

        MZSDL_PutPixel32(BrushSurface,centerX+x,centerY-y,color|alpha2);
        MZSDL_PutPixel32(BrushSurface,centerX-x,centerY-y,color|alpha2);
        TOOL_FillLine(BrushSurface,centerX-x+1,centerY-y,centerX+x-1);

        MZSDL_PutPixel32(BrushSurface,centerY+y,centerX+x,color|alpha2);
        MZSDL_PutPixel32(BrushSurface,centerY-y,centerX+x,color|alpha2);
        TOOL_FillLine(BrushSurface,centerY-y+1,centerX+x,centerY+y-1);

        MZSDL_PutPixel32(BrushSurface,centerY+y,centerX-x,color|alpha2);
        MZSDL_PutPixel32(BrushSurface,centerY-y,centerX-x,color|alpha2);
        TOOL_FillLine(BrushSurface,centerY-y+1,centerX-x,centerY+y-1);


        alpha=255*LINE_FPart(y) ;
        alpha<<=ashift;

        if(y>x)
        {
            //outside
            MZSDL_PutPixel32(BrushSurface,centerX+x,centerY+y+1 ,color|alpha);
            MZSDL_PutPixel32(BrushSurface,centerX+x,centerY-y-1,color|alpha);
            MZSDL_PutPixel32(BrushSurface,centerX-x,centerY+y+1,color|alpha);
            MZSDL_PutPixel32(BrushSurface,centerX-x,centerY-y-1,color|alpha);

            MZSDL_PutPixel32(BrushSurface,centerY+y,centerX+x+1,color|alpha);
            MZSDL_PutPixel32(BrushSurface,centerY-y,centerX+x+1,color|alpha);
            MZSDL_PutPixel32(BrushSurface,centerY+y,centerX-x-1,color|alpha);
            MZSDL_PutPixel32(BrushSurface,centerY-y,centerX-x-1,color|alpha);

        }
        else
        {
            //outside
            MZSDL_PutPixel32(BrushSurface,centerX+x+1,centerY+y ,color|alpha);
            MZSDL_PutPixel32(BrushSurface,centerX+x+1,centerY-y,color|alpha);
            MZSDL_PutPixel32(BrushSurface,centerX-x-1,centerY+y,color|alpha);
            MZSDL_PutPixel32(BrushSurface,centerX-x-1,centerY-y,color|alpha);

            MZSDL_PutPixel32(BrushSurface,centerY+y+1,centerX+x,color|alpha);
            MZSDL_PutPixel32(BrushSurface,centerY-y-1,centerX+x,color|alpha);
            MZSDL_PutPixel32(BrushSurface,centerY+y+1,centerX-x,color|alpha);
            MZSDL_PutPixel32(BrushSurface,centerY-y-1,centerX-x,color|alpha);

        }

    }


}

void TOOL_UseBrush(int x,int y)
{
    //POINTER_PEN_INFO  tabletPen;
    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);
    SDL_Event event;

    SDL_Thread * updateTH,*drawTH;

    float spaceLength = (size * 2 * space)/100;
    spaceLength *= spaceLength;//square value for calculating length
    if(spaceLength<1)
    {
        spaceLength=1;
    }
printf("sp=%g\n",spaceLength);
    int A,B,xx,yy;
    int Res;

    SDL_AtomicLock(&THREADONBRUSH_lock);
    THREAD_ON_BRUSH = 1;
    SDL_AtomicUnlock(&THREADONBRUSH_lock);

    tempSurface=SDL_CreateRGBSurface(0, mainFile->w, mainFile->h, 32, rmask, gmask, bmask, amask);

    DF_GetMouseFromLayer(&x,&y);

    SDL_Point * pos =(SDL_Point *) malloc(sizeof(SDL_Point));
    pos->x=x;
    pos->y=y;

    updateTH=SDL_CreateThread(TOOL_ThreadBrushUpdate,"updateThread",NULL);
    drawTH=SDL_CreateThread(TOOL_PointsDrawThread,"drawThread",NULL);


    TOOL_ADDBrushPoint(x,y,size,100);

    while( SDL_WaitEvent(&event))
    {
        if(event.type==SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&x,&y);
            DF_GetMouseFromLayer(&x,&y);

            A=x-pos->x;
            B=y-pos->y;
            Res=A*A + B*B;

            while(Res >= spaceLength)
            {
                //calculate points
                xx=round((A*A*spaceLength)/Res);

                xx=sqrt(xx);
                yy=round((B*B*spaceLength)/Res);
                yy=sqrt(yy);
                if(A>0)
                {
                    xx=pos->x +xx;
                }
                else
                {
                    xx=pos->x-xx;
                }
                if(B>0)
                {
                    yy=pos->y +yy;
                }
                else
                {
                    yy=pos->y-yy;
                }

                TOOL_ADDBrushPoint(xx,yy,size,100);
                pos->x=xx;
                pos->y=yy;

                //recalculate the remaining length
                A=x-xx;
                B=y-yy;
                Res= A*A + B*B;
            }
        }
        else if(event.type==SDL_MOUSEBUTTONUP)
        {
            break;
        }
    }

    SDL_AtomicLock(&THREADONBRUSH_lock);
    THREAD_ON_BRUSH = 0;
    SDL_AtomicUnlock(&THREADONBRUSH_lock);

    //finish all processors
    SDL_WaitThread(drawTH,&x);
    SDL_WaitThread(updateTH,&x);

    //apply to layer
    DR_SetDrawableBuffer(tempSurface);
    DR_BlitDrawable();

    DK_UpdateLayerDock();

    SDL_FreeSurface(tempSurface);
    if(tempTexture!=NULL)
        SDL_DestroyTexture(tempTexture);
    free(pos);
}

int TOOL_ThreadBrushUpdate(void * data)
{
    SDL_Rect srect,drect;

    if(tempSurface==NULL)
    {
        return 0;
    }

   while(1)
   {
        SDL_AtomicLock(&THREADONBRUSH_lock);
        if(!THREAD_ON_BRUSH)
        {
            SDL_AtomicUnlock(&THREADONBRUSH_lock);

            SDL_AtomicLock(&BrushPointHead_lock);
            if(BrushPointHead==NULL)
            {
                SDL_AtomicUnlock(&BrushPointHead_lock);
                break;
            }
            SDL_AtomicUnlock(&BrushPointHead_lock);
        }
        SDL_AtomicUnlock(&THREADONBRUSH_lock);

       SDL_AtomicLock(&RENDERUPDATE_lock);
       if(RENDERUPDATE)
       {
           SDL_AtomicUnlock(&RENDERUPDATE_lock);

           //manage veiwports
            DR_MainViewPort(renderer, &VP_Texture);

            if(tempTexture)
            {
                SDL_DestroyTexture(tempTexture);
            }
            tempTexture=TOOL_CreateLineTexture(tempSurface,&srect,&drect);

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer,BGTexture,NULL,NULL);
            SDL_RenderCopy(renderer,VP_Texture,NULL,NULL);

            SDL_RenderCopy(renderer,tempTexture,&srect,&drect);//check rect to solve the problem

            if(GLOBAL_SELECTION_LINES)
            {
                SDL_RenderCopy(renderer,GLOBAL_SELECTION_LINES,&GLOBAL_SELECTION_LINES_SOURCE_Rect,&GLOBAL_SELECTION_LINES_DEST_Rect);
            }
            SDL_RenderPresent(renderer);
            SDL_AtomicLock(&RENDERUPDATE_lock);
            RENDERUPDATE=0;//reset the var
        }
        else
        {

        }
        SDL_AtomicUnlock(&RENDERUPDATE_lock);
    }

    return 1;
}

int TOOL_ADDBrushPoint(int x,int y,int bsize,int alpha)
{
    //SDL_Thread * Points;
    TOOL_BrushPoint * tmp;
    TOOL_BrushPoint * point=(TOOL_BrushPoint *) malloc(sizeof(TOOL_BrushPoint));
    if(point==NULL)
    {
        return 0;
    }

    point->x=x;
    point->y=y;
    point->size=bsize;
    point->alpha=alpha;
    point->next=NULL;


    SDL_AtomicLock(&BrushPointHead_lock);

    if(BrushPointHead==NULL)
    {
        BrushPointHead=point;
    }
    else
    {
        tmp=BrushPointHead;
        while(tmp->next)
        {
            tmp=tmp->next;
        }
        tmp->next=point;
    }
    SDL_AtomicUnlock(&BrushPointHead_lock);
    return 1;
}

int TOOL_PointsDrawThread(void * data)
{
    TOOL_BrushPoint *tmp;
    TOOL_BrushPoint cur;
    while(1)
    {
        SDL_AtomicLock(&THREADONBRUSH_lock);
        if(!THREAD_ON_BRUSH)
        {
            if(BrushPointHead==NULL)
            {
                SDL_AtomicUnlock(&THREADONBRUSH_lock);
                break;
            }
        }
        SDL_AtomicUnlock(&THREADONBRUSH_lock);
        SDL_AtomicLock(&BrushPointHead_lock);
        if(BrushPointHead==NULL)
        {
            SDL_AtomicUnlock(&BrushPointHead_lock);
            continue;
        }

        cur.alpha=BrushPointHead->alpha;
        cur.size=BrushPointHead->size;
        cur.x=BrushPointHead->x;
        cur.y=BrushPointHead->y;

        tmp=BrushPointHead;
        BrushPointHead=BrushPointHead->next;
        SDL_AtomicUnlock(&BrushPointHead_lock);

        free(tmp);
        tmp=NULL;

        //draw point
        TOOL_DrawBrush(tempSurface,cur.x,cur.y);
        DR_SetBufferToDrawableData(tempSurface);

        SDL_AtomicLock(&RENDERUPDATE_lock);
        RENDERUPDATE = 1;
        SDL_AtomicUnlock(&RENDERUPDATE_lock);
    }
    return 1;
}

void TOOL_FillLine(SDL_Surface * surface,int x0,int y,int x1)
{
    int i;
    Uint32 color=SYS_GetForegroundColor() | amask;
    for(i=x0;i<=x1;i++)
    {
        MZSDL_PutPixel32(surface,i,y,color);
    }
}

void TOOL_DrawBrush(SDL_Surface *surface,int x, int y)
{

    int i,j;
    int u=0,v=0,v0=0;
    Uint8 a,asrc,adst;
    Uint8 r,g,b;
    double dA,dS,dD;
    int x0=x-Xcenter,x1=x+Xcenter;
    int y0=y-Ycenter,y1=y+Ycenter;

    //check the borders
    if(x0<0)
    {
        u=-x0;
        x0=0;
    }
    if(x1>mainFile->w)
    {
        x1=mainFile->w;
    }
    if(y0<0)
    {

        v=-y0;
        v0=v;//reseting value
        y0=0;
    }
    if(y1>mainFile->h)
    {
        y1=mainFile->h;
    }


    for(i=x0;i<x1;i++)
    {
        for(j=y0;j<y1;j++)
        {
            asrc=SYS_GetAlphaXY(BrushSurface,u,v);
            if(asrc==0)
            {
                v++;
                continue;
            }

            if(asrc!=0xff)
            {
                adst=SYS_GetAlphaXY(surface,i,j);
                a=asrc + adst*(255-asrc)/255;

                dA = a      / 255.0;
                dS = asrc   / 255.0;
                dD = adst   / 255.0;

                r=(SYS_GetRedXY(BrushSurface,u,v)*dS      +   SYS_GetRedXY(surface,i,j)  * dD *(1-dS))/dA;//check more
                g=(SYS_GetGreenXY(BrushSurface,u,v)*dS    +   SYS_GetGreenXY(surface,i,j)* dD *(1-dS))/dA;
                b=(SYS_GetBlueXY(BrushSurface,u,v)*dS     +   SYS_GetBlueXY(surface,i,j) * dD *(1-dS))/dA;
            }
            else
            {
                a=asrc;
                r=SYS_GetRedXY(BrushSurface,u,v);
                g=SYS_GetGreenXY(BrushSurface,u,v);
                b=SYS_GetBlueXY(BrushSurface,u,v);
            }
            MZSDL_PutPixel32(surface,i,j,SYS_RGBA(r,g,b,a));
            v++;
        }
        u++;
        v=v0;
    }
}


void TOOL_UnSetBrush(DR_Tools * brush)
{
    SDL_FreeSurface(BrushSurface);
    SDL_FreeCursor(brush->cursor);

    BrushSurface=NULL;
    brush->cursor=NULL;
    TOOLOPTION_DestroyOptionBar();

}

void TOOL_UpdateBrushSurface(Uint32 color)
{
    int i,j;
    int x=BrushSurface->w,y=BrushSurface->h;
    Uint8 a;
    color = color & ~amask;
    Uint32 fcolor;
    for(i=0;i<x;i++)
    {
        for(j=0;j<y;j++)
        {
            a=SYS_GetAlphaXY(BrushSurface,i,j);
            if(a==0)
            {
                continue;
            }

            fcolor = color | a<<ashift;
            MZSDL_PutPixel32(BrushSurface,i,j,fcolor);
        }
    }
}

void TOOL_BrushSetValueSize(int value)
{
    size = value;

    //update brush
    SDL_FreeSurface(BrushSurface);
    SDL_FreeCursor(DR_GetTool()->cursor);

    SDL_Surface * surface=SDL_CreateRGBSurface(0,size*2+2,size*2+2,32,rmask,gmask,bmask,amask);

    BrushSurface=SDL_CreateRGBSurface(0,size*2+2,size*2+2,32,rmask,gmask,bmask,amask);
    Xcenter=size+1;
    Ycenter=size+1;
    TOOL_DrawCircle(surface,size,Xcenter,Ycenter);
    DR_GetTool()->cursor=SDL_CreateColorCursor(surface,Xcenter,Ycenter);
    if(DR_GetTool()->cursor==NULL)
    {
        printf("error in cursor:%s\n",SDL_GetError());
    }

}

void TOOL_BrushBoxSetValueSize( int value)
{
    TOOL_BrushSetValueSize(value);
}

void TOOL_BrushBoxSetValueSpace(int value)
{
    space=value;
}

void TOOL_BrushSetValueSpace( int value)
{
    TOOL_BrushSetValueSize(value);
}

void TOOL_SetValueMode(int value)
{
    TOOL_Mode=value;
}
