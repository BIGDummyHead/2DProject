//
// Created by shawn on 5/16/2025.
//

#include "Init.h"

#include "SDL_ttf/include/SDL_ttf.h"

void Init::initSDL(App* app) {
    constexpr int renderFlag = SDL_RENDERER_ACCELERATED;
    constexpr int windowFlag = 0;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL Renderer");
        exit(-1);
    }

    if(app->name == nullptr) {
        printf("Please provide an application name before initializing");
        exit(-1);
    }

    auto* window =
        SDL_CreateWindow(app->name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlag);

    if(!window) {
        printf("SDL Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(-1);
    }

    app->setWindow(window);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == -1) {
        printf("Failed to initialize SDL_Img: %s\n", IMG_GetError());
        exit(-1);
    }

    if(TTF_Init() == -1) {
        printf("Failed to initialize SDL_ttf: %s\n", TTF_GetError());
        exit(-1);
    }

    auto* renderer = SDL_CreateRenderer(window, -1, renderFlag);

    if(!renderer) {
        printf("Failed to create the renderer: %s\n", SDL_GetError());
        exit(-1);
    }

    app->setRenderer(renderer);
}


