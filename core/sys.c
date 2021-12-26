#include "sys.h"
#include <SDL.h>


Uint32 SYS_RGBA(Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
    return r << rshift | g << gshift | b << bshift | a << ashift;
}
Uint8 SYS_GetAlphaXY(SDL_Surface * surface,int x, int y )
{
    Uint8 alpha;
    Uint32 * pixels= (Uint32 *)surface->pixels;
    Uint32 pixel=pixels[( y * surface->w ) + x];
    pixel= pixel & amask;
    alpha= pixel >> ashift;

    return alpha;
}

Uint8 SYS_GetAlpha(SDL_Surface * surface,int i )
{
    Uint8 alpha;
    Uint32 * pixels=(Uint32 *)surface->pixels;
    Uint32 pixel=pixels[i];
    pixel= pixel & amask;

    alpha= pixel >> ashift;

    return alpha;
}

void SYS_SetAlpha(SDL_Surface * surface,int i ,Uint8 a)
{

    Uint32 * pixels=(Uint32 *)surface->pixels;
    Uint32 pixel=pixels[i] & ~amask;
    pixel = pixel | a<<ashift;

    pixels[i]=pixel;
}

//
Uint8 SYS_GetRed(SDL_Surface * surface,int i )
{
    Uint8 red;
    Uint32 * pixels=(Uint32 *)surface->pixels;
    Uint32 pixel=pixels[i];
    pixel= pixel & rmask;
    red= pixel >> rshift;

    return red;
}

Uint8 SYS_GetRedXY(SDL_Surface * surface,int x, int y )
{
    Uint8 red;
    Uint32 * pixels= (Uint32 *)surface->pixels;
    Uint32 pixel=pixels[( y * surface->w ) + x];
    pixel= pixel & rmask;
    red= pixel >> rshift;

    return red;
}

void SYS_SetRed(SDL_Surface * surface,int i ,Uint8 r)
{

    Uint32 * pixels=(Uint32 *)surface->pixels;
    Uint32 pixel=pixels[i] & ~rmask;
    pixel = pixel | r<<rshift;

    pixels[i]=pixel;
}
//
Uint8 SYS_GetGreen(SDL_Surface * surface,int i )
{
    Uint8 green;
    Uint32 * pixels=(Uint32 *)surface->pixels;
    Uint32 pixel=pixels[i];
    pixel= pixel & gmask;
    green= pixel >> gshift;

    return green;
}

Uint8 SYS_GetGreenXY(SDL_Surface * surface,int x, int y )
{
    Uint8 green;
    Uint32 * pixels= (Uint32 *)surface->pixels;
    Uint32 pixel=pixels[( y * surface->w ) + x];
    pixel= pixel & gmask;
    green= pixel >> gshift;

    return green;
}

void SYS_SetGreen(SDL_Surface * surface,int i ,Uint8 g)
{

    Uint32 * pixels=(Uint32 *)surface->pixels;
    Uint32 pixel=pixels[i] & ~gmask;
    pixel = pixel | g<<ashift;

    pixels[i]=pixel;
}
//
Uint8 SYS_GetBlue(SDL_Surface * surface,int i )
{
    Uint8 blue;
    Uint32 * pixels=(Uint32 *)surface->pixels;
    Uint32 pixel=pixels[i];
    pixel= pixel & bmask;
    blue= pixel >> bshift;

    return blue;
}

Uint8 SYS_GetBlueXY(SDL_Surface * surface,int x, int y )
{
    Uint8 blue;
    Uint32 * pixels= (Uint32 *)surface->pixels;
    Uint32 pixel=pixels[( y * surface->w ) + x];
    pixel= pixel & bmask;
    blue= pixel >> bshift;

    return blue;
}

void SYS_SetBlue(SDL_Surface * surface,int i ,Uint8 b)
{

    Uint32 * pixels=(Uint32 *)surface->pixels;
    Uint32 pixel=pixels[i] & ~amask;
    pixel = pixel | b<<bshift;

    pixels[i]=pixel;
}

void SYS_SetPixel( SDL_Surface *surface, int i, Uint32 pixel )
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)surface->pixels;
	//Set the pixel
	pixels[i] = pixel;
}

void MZSDL_PutPixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)surface->pixels;
	//Set the pixel
	if(x<surface->w && y<surface->h)

	pixels[ ( y * surface->w ) + x ] = pixel;
}

int SYS_InsideRect(int x,int y, SDL_Rect * rect)
{
    if(rect->x<x && rect->x+rect->w>x)
   {
        if(rect->y<y && rect->y+rect->h>y)
        {
            return 1;
        }
   }
    return 0;
}

void SYS_DrawVerticalLine(SDL_Surface * surface,int x,int y0,int y1,Uint32 color)
{
    int i;
    for(i=y0;i<=y1;i++)
    {
        MZSDL_PutPixel32(surface,x,i,color);
    }
}

void SYS_DrawHorizantalLine(SDL_Surface * surface,int x0,int x1,int y,Uint32 color)
{
    int i;
    for(i=x0;i<=x1;i++)
    {
        MZSDL_PutPixel32(surface,i,y,color);
    }
}

void SYS_DrawOpactiy_0(SDL_Surface * surface,SDL_Rect * dest,int size)
{
    int i,j;
    int x,y,w,h;

    SDL_Rect rect,rect2;

    rect.x=0;
    rect.y=0;
    rect.w=size;
    rect.h=size;

    rect2.x=size;
    rect2.y=size;
    rect2.w=size;
    rect2.h=size;

    if(dest)
    {
        x=dest->x;
        y=dest->y;
        w=dest->w + x;
        h=dest->h + y;
        //puts("---------------------------------------------------------------------------");
    }
    else
    {
        x=0;
        y=0;
        w=surface->w;
        h=surface->h;
    }
//printf("x=%d\ty=%d\tw=%d\th=%d\tsize=%d\n",x,y,w,h,size);
    int size2=size +size;

    SDL_FillRect(surface,dest,0xffffffff);

    for(i=x;i<w;i+=size2)
    {
        rect.x=i;
        rect2.x=i+size;
        for(j=y;j<h;j+=size2)// minus the size
        {
            rect.y=j;
            rect2.y=j+size;
            SDL_FillRect(surface,&rect,SYS_RGBA(0xd0,0xd0,0xd0,0xff));
            SDL_FillRect(surface,&rect2,SYS_RGBA(0xd0,0xd0,0xd0,0xff));
           // printf("x=%d\ty=%d\tsize=%d\n",i,j,size);

        }
    }
}


