//
// Created by shawn on 5/16/2025.
//

#include "Vector2.h"


SDL_Rect Vector2::asRect() const {
    SDL_Rect rect;

    //ignore truncation
    rect.x = x;
    rect.y = y;

    return rect;
}
