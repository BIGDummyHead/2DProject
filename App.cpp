//
// Created by shawn on 7/26/2025.
//

#include "App.h"

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



