//
// Created by shawn on 5/16/2025.
//

#include "input.h"
#include <unordered_map>
#include <iostream>
#include <bits/algorithmfwd.h>

#include "defs.h"

std::unordered_map<SDL_Keycode, bool> isDown;
std::unordered_map<SDL_Keycode, bool> isUp;
std::unordered_map<SDL_Keycode, bool> holding;
std::unordered_map<Uint8, MouseState> mouseState;
std::unordered_map<Uint8, MouseState> lastMouseState;

DWORD input::currentPID = 0;
HWND input::windowHandle = nullptr;


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

void input::updateMouseInputState(const Uint8 &key, const bool& down) {
    mouseState[key] = down ? Down : Up;
}


MouseState input::getMouseInputState(const MouseButton &mouseButton) {

    Uint8 key;
    switch (mouseButton) {
        case Left:
            key = SDL_BUTTON_LEFT;
            break;
        case Right:
            key = SDL_BUTTON_RIGHT;
            break;;
        default:
            throw std::runtime_error("Invalid choice");
    }

    if(!mouseState.contains(key)) {
        return Up; //assume it has never been pressed or anything
    }

    MouseState state = mouseState[key];

    if(lastMouseState[key] == Down && state == Down) {
        mouseState[key] = Held;
        state = Held;
    }
    lastMouseState[key] = state;



    return state;
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
            case SDL_MOUSEBUTTONDOWN:
                updateMouseInputState(event.button.button, true);
                break;
            case SDL_MOUSEBUTTONUP:
                updateMouseInputState(event.button.button, false);
            default:
                break;
        }

    }
}

long input::clampLong( const long& val,  const long& min, const long& max) {
    if(val < min)
        return min;
    else if(val > max)
        return max;

    return val;
}


Vector2 input::getMousePosition() {

    if(!windowHasFocus()) {
        return Vector2{ 0, 0 };
    }

    Vector2 finalPosition;

    POINT cursorPoint;
    HWND currentHandle = windowHandle;
    if(GetCursorPos(&cursorPoint)) {
        if(ScreenToClient(currentHandle, &cursorPoint)) {
            finalPosition.x = clampLong(cursorPoint.x, 0, SCREEN_WIDTH);
            finalPosition.y = clampLong(cursorPoint.y, 0, SCREEN_HEIGHT);
        }
    }


    return finalPosition;
}


//SOURCE: https://stackoverflow.com/questions/11711417/get-hwnd-by-process-id-c
BOOL CALLBACK input::EnumWindowsProcMy(HWND hwnd, LPARAM lParam) {
    DWORD lpdwProcessId;
    GetWindowThreadProcessId(hwnd,&lpdwProcessId);
    if(lpdwProcessId==lParam)
    {
        windowHandle = hwnd;
        return FALSE;
    }
    return TRUE;
}

bool input::windowHasFocus() {

        currentPID = GetCurrentProcessId(); //retrieve the current process id
        EnumWindows(EnumWindowsProcMy, currentPID);

    return GetForegroundWindow() == windowHandle;
}





