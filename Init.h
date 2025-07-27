//
// Created by shawn on 5/16/2025.
//

#ifndef INIT_H
#define INIT_H

#include <cstdio>
#include <SDL.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <iostream>
#include "SDL_image/include/SDL2/SDL_image.h"
#include "App.h"

#include "defs.h"

//SDL Main Initialization logic
class Init {
public:

    static void initSDL(App* app);


};



#endif //INIT_H
