#include "DR-defs.h"
#include <SDL.h>

//Screen dimension constants
int GLOBAL_SCREEN_WIDTH = 1280;
int GLOBAL_SCREEN_HEIGHT = 720;

//quit the main loop variable
char GLOBAL_QUIT_PROGRAM = false;

//selection vars

SDL_Texture * GLOBAL_SELECTION_LINES=NULL;
SDL_Rect GLOBAL_SELECTION_LINES_SOURCE_Rect;
SDL_Rect GLOBAL_SELECTION_LINES_DEST_Rect;

