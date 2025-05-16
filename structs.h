//
// Created by shawn on 5/16/2025.
//

#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL_video.h>
#include <SDL_render.h>


typedef struct {
    const char* name;
    SDL_Renderer *renderer;
    SDL_Window *window;
} App;

#endif //STRUCTS_H
