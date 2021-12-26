#ifndef _FILE_H_
#define _FILE_H_

#include <SDL2/SDL.h>

typedef struct DF_Layer
{
    char view;
    char lockAlpha;
    char lockPaint;
    char opacity;

    char name[32];
    SDL_Surface * surface;

    struct DF_Layer * next;
} DF_Layer;

typedef struct
{
    int w;
    int h;

    int zoom;
    int layerCount;//used to counted and name the new layer
    SDL_Surface * surface;
    SDL_Rect rect;

    DF_Layer * layer;

    DF_Layer * activeLayer;

    char name[64];
} DF_File;

extern DF_File * mainFile;

int DF_CreateNewFile(int w, int h);
void DF_CreateScrollDrawing();

void DF_CheckFileRect();
void DF_UpdateFile();

void DF_GetMouseFromLayer(int * x, int * y);

#endif // _FILE_H_
