//
// Created by shawn on 5/16/2025.
//

#ifndef INPUT_H
#define INPUT_H
#include <SDL_events.h>
#include <process.h>

//Class to deal with input
class input {

public:
    static void onKeyDown(SDL_Keycode pressed);
    static void onKeyUp(SDL_Keycode released);
    static bool isKeyDown(SDL_Keycode key);
    static bool isKeyUp(SDL_Keycode key);
    static void pollInput();
    static void doKeyCheck(const SDL_Event &event);
    static void onKeyHeld(SDL_Keycode pressing);
    static  bool isKeyHeld(SDL_Keycode key);
};



#endif //INPUT_H
