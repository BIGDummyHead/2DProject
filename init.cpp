//
// Created by shawn on 5/16/2025.
//

#include "init.h"

void init::initSDL(App& app) {
    constexpr int renderFlag = SDL_RENDERER_ACCELERATED;
    constexpr int windowFlag = 0;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL Renderer");
        exit(-1);
    }

    if(app.name == nullptr) {
        printf("Please provide an application name before initializing");
        exit(-1);
    }

    app.window = SDL_CreateWindow(app.name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlag);

    if(!app.window) {
        printf("SDL Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(-1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    app.renderer = SDL_CreateRenderer(app.window, -1, renderFlag);

    if(!app.renderer) {
        printf("Failed to create the renderer: %s\n", SDL_GetError());
        exit(-1);
    }
}


