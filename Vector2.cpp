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

Vector2 Vector2::normalize() const {
    const double length = sqrt(x * x + y * y);
    return length != 0 ? Vector2{x / length, y / length} : Vector2();
}


double Vector2::magnitude() const {
    return sqrt(pow(x, 2) + pow(y, 2));
}

Vector2 Vector2::absolute() const {
    return Vector2{fabs(x), fabs(y)};
}

Vector2 Vector2::normalized() const {

    const double mag = magnitude();
    return mag == 0 ? Vector2{0,0} : Vector2{ x / mag, y / mag };
}

double Vector2::length() const {
    return sqrt(x * x + y * y);
}



