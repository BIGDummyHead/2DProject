//
// Created by shawn on 5/16/2025.
//

#include "Vector2.h"
#include <cmath>

SDL_Rect Vector2::asRect() const {
    SDL_Rect rect;

    //ignore truncation
    rect.x = x;
    rect.y = y;

    return rect;
}

double Vector2::distance(const Vector2 &other) const {
    //distance = sqrt( (x2 - x1)^2 + (y2 - y1)^2 )
    return sqrt(pow(other.x - x, 2) + pow(other.y - y, 2));
}

