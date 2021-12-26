#ifndef _DR_VARS_H_
#define _DR_VARS_H_

#include <SDL2/SDL.h>

//Screen dimension constants
extern int GLOBAL_SCREEN_WIDTH ;
extern int GLOBAL_SCREEN_HEIGHT;

//quit the main loop variable
extern char GLOBAL_QUIT_PROGRAM;

extern SDL_Renderer * renderer;
extern SDL_Texture * BGTexture;
extern SDL_Texture * VP_Texture;

//selection vars
extern SDL_Texture * GLOBAL_SELECTION_LINES;
extern SDL_Rect GLOBAL_SELECTION_LINES_SOURCE_Rect;
extern SDL_Rect GLOBAL_SELECTION_LINES_DEST_Rect;
#endif // _DR_VARS_H_
