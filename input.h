//
// Created by shawn on 5/16/2025.
//

#ifndef INPUT_H
#define INPUT_H
#include <SDL_events.h>
#include <process.h>
#include "Vector2.h"
#include <windows.h>
#include <cmath>

enum MouseState {
    Up,
    Down,
    Held
};

enum MouseButton {
    Left,
    Right,
    //others to come soon
};

enum Direction {
    Vertical,
    Horizontal
};

//Class to deal with input
class input {

private:
    static DWORD currentPID;
    static HWND windowHandle;
    static BOOL CALLBACK EnumWindowsProcMy(HWND hwnd,LPARAM lParam);
    static double horizontalSmoother;
    static double verticalSmoother;

public:
    static void onKeyDown(SDL_Keycode pressed);
    static void onKeyUp(SDL_Keycode released);
    static bool isKeyDown(SDL_Keycode key);
    static bool isKeyUp(SDL_Keycode key);
    static void pollInput();
    static void doKeyCheck(const SDL_Event &event);
    static void onKeyHeld(SDL_Keycode pressing);
    static  bool isKeyHeld(SDL_Keycode key);
    static Vector2 getMousePosition();
    static  bool windowHasFocus();
    static long clampLong( const long& val,  const long& min, const long& max);

    static MouseState getMouseInputState(const MouseButton& mouseButton);
    static void updateMouseInputState(const Uint8& key, const bool& down);

    static double getMovement(const Direction& direction);
};



#endif //INPUT_H
