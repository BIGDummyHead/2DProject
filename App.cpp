//
// Created by shawn on 7/26/2025.
//

#include "App.h"

#include <cstdio>
#include <SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Draw.h"

App* App::applicationInstance;

SDL_Renderer *App::getRenderer() const {
    return renderer;
}

SDL_Window *App::getWindow() const {
    return window;
}

void App::setRenderer(SDL_Renderer *renderer) {
    this->renderer = renderer;
}

void App::setWindow(SDL_Window *window) {
    this->window = window;
}

App *App::getInstance() {
    return applicationInstance;
}

const char* App::initialize(App::Settings* settings) {

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        return SDL_GetError();
    }

    if(name.length() == 0) {
        return SDL_GetError();
    }

    auto* window =
        SDL_CreateWindow(name.c_str(),
            settings->windowPosition.x,
            settings->windowPosition.y,
            settings->windowDimensions.x,
            settings->windowDimensions.y, settings->windowFlags);

    if(!window) {
        return SDL_GetError();
    }

    setWindow(window);

    for(auto& hint : settings->sdlHints) {
        if(SDL_SetHint(hint.name.c_str(), hint.value.c_str()) != SDL_TRUE) {
            hint.failed = true;
        }
    }

    if(IMG_Init(settings->imageFlags) == -1) {
        return SDL_GetError();
    }

    if(TTF_Init() == -1) {
        return SDL_GetError();
    }

    auto* renderer = SDL_CreateRenderer(window, settings->index, settings->renderFlags);

    if(!renderer) {
        return SDL_GetError();
    }

    setRenderer(renderer);

    //create a new Draw tool instance, uses singleton type behaviour
    new Draw();

    this->settings = settings;

    return nullptr;
}

App::Settings *App::getSettings() const {
    return settings;
}





