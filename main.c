#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "DR-vars.h"
#include "DR-defs.h"
#include "DR-event.h"
#include "DR-viewport.h"
#include "mainViewport.h"
#include "core/sys.h"
#include "core/color.h"

#include "file/file.h"
#include "tools/tools.h"
#include "tools/toolbar.h"
#include "file/drawable.h"
#include "navigator/navigator.h"


SDL_Renderer * renderer=NULL;
SDL_Texture * BGTexture=NULL;
SDL_Texture * VP_Texture=NULL;

int main( int argc, char* args[] )
{
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;


	//sdl_image
	int flags=IMG_INIT_JPG|IMG_INIT_PNG;
	int initted;
	//the event handler
	SDL_Event  EventH;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );//we will change it later to a error windows
        return -1;
	}
	 if(TTF_Init()==-1)
    {
        printf("Can't initailize ttf\n");
    }



    initted=IMG_Init(flags);
    if(!(initted&flags) )
    {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());// handle error
    }

    //Create window
    window = SDL_CreateWindow( "Vetro", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GLOBAL_SCREEN_WIDTH, GLOBAL_SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
    if( window == NULL )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return -1;
    }

    renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer==NULL)
    {
         printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
         return -1;
    }

    screenSurface=SDL_CreateRGBSurface(0,GLOBAL_SCREEN_WIDTH,GLOBAL_SCREEN_HEIGHT,32,rmask,gmask,bmask,amask);
    if(screenSurface==NULL)
    {
         printf( "screen surface could not be created! SDL_Error: %s\n", SDL_GetError() );
        return -1;
    }
    //SDL_FillRect(screenSurface,NULL,SDL_MapRGB( screenSurface->format, 0xf, 0x80, 0x80 ));
    SYS_DrawOpactiy_0(screenSurface,NULL,12);
    //SDL_FillRect(screenSurface,NULL,SYS_RGBA(0x230,0,0,0xff));
    BGTexture=SDL_CreateTextureFromSurface(renderer,screenSurface);
    if(BGTexture==NULL)
    {
         printf( "back ground texture could not be created! SDL_Error: %s\n", SDL_GetError() );
        return -1;
    }
    SDL_FreeSurface(screenSurface);//not needed anymore

    //create the main view ports
    MAIN_CreateMainViewport();

    DF_CreateNewFile(500,500);

    if(!DR_InitTools())
    {
        return -1;
    }

    if(!DR_CreateToolBar())
    {
        return -1;
    }
    if(!DR_CreateNavigator())
    {
        return -1;
    }

    DR_Initdrawable();


    while(!GLOBAL_QUIT_PROGRAM)
    {
        if(SDL_WaitEvent(&EventH))//maybe here
        {
            DR_EventHandler(&EventH);
        }
        //manage veiwports
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

    DR_QuitViewPorts();
	SDL_DestroyTexture(BGTexture);
	SDL_DestroyTexture(VP_Texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}
