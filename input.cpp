//
// Created by shawn on 5/16/2025.
//

#include "input.h"
#include <unordered_map>
#include <iostream>

std::unordered_map<SDL_Keycode, bool> isDown;
std::unordered_map<SDL_Keycode, bool> isUp;
std::unordered_map<SDL_Keycode, bool> holding;

void input::onKeyDown(const SDL_Keycode pressed) {
    if(const auto iterVal = isDown.find(pressed); iterVal == isDown.end()) {
        //add the key here as it does not exist
        isDown[pressed] = true;
        isUp[pressed] = false; //add this because how could something be up if it has not been pressed?
        return;
    }

    isDown[pressed] = true;
    isUp[pressed] = false;


}

void input::onKeyUp(const SDL_Keycode released) {
    if(const auto iterVal = isUp.find(released); iterVal == isUp.end()) {
        isUp[released] = true;
        return;
    }

    isUp[released] = true;
    holding[released] = false;
    isDown[released] = false;

}

void input::onKeyHeld(const SDL_Keycode pressing) {

    isDown[pressing] = false;
    holding[pressing] = true;

}

bool input::isKeyDown(SDL_Keycode key) {
    const auto find = isDown.find(key);
    return find != isDown.end() && isDown[key];
}

bool input::isKeyUp(const SDL_Keycode key) {
    const auto find = isUp.find(key);
    return find == isUp.end() || isUp[key];
}

bool input::isKeyHeld(const SDL_Keycode key) {
    return isKeyDown(key) || holding.contains(key) && holding[key];
}


void input::doKeyCheck(const SDL_Event& event) {

    //check if isDown contains the keycode

    const SDL_Keycode key = event.key.keysym.sym;

    //if isDown does not contain we shall add it
    //or
    //if the current key is not down and it is also not being held
    if(!isDown.contains(key) || (!isDown[key] && !holding[key])) {
        onKeyDown(key);
    }
    else { //item was down in the previous poll
        onKeyHeld(key);
    }
}

void input::pollInput() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
            break;
            case SDL_KEYDOWN:
                doKeyCheck(event);
                break;
            case SDL_KEYUP:
                onKeyUp(event.key.keysym.sym);
                break;
            default:
                break;
        }

    }
}



