#ifndef _DR_VIEWPORT_H_
#define _DR_VIEWPORT_H_

#include <SDL2/SDL.h>
#include <stdio.h>

#define fixed 1
#define dynamic 2

#define top 0
#define bottom 1
#define left 0
#define right 1
#define undefined -2


//veiwport struct
typedef struct{
int hieght;
int width;
char hieghtData;
char widthData;
char HAlign;
char VAlign;
int bgcolor;//if you like to add a background from here
}DR_Viewport;

typedef struct DR_MainViewport{//this sturct will control every viewport
DR_Viewport * viewport;
SDL_Rect  rect;
SDL_Surface * surface;
char name[16];
struct DR_MainViewport * next;
}DR_MainViewport;

//Global variable
//extern DR_MainViewport * VP_Manager;

//veiwport functions
void DR_InitViewport();
void DR_MainViewPort(SDL_Renderer * ren, SDL_Texture ** Texture);//the view port manager
void DR_CreateViewPort(DR_Viewport * newport,const char * name);
void DR_CreateViewPortInMain(DR_Viewport * newport,SDL_Rect,const char * name);

void DR_ResizeViewPort(DR_MainViewport * port);
void DR_ResizeViewPortInMain(DR_MainViewport * port);

void DR_QuitViewPorts();

DR_MainViewport * DR_FindViewport(const char * name);
DR_MainViewport * DR_GetViewportHead();

#endif // _DR_VIEWPORT_H_
