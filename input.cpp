//
// Created by shawn on 5/16/2025.
//

#include "input.h"


void input::doInput() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
            break;
            default:
                break;
        }
    }
}
