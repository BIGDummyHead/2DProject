//
// Created by shawn on 5/16/2025.
//

#ifndef VECTOR2_H
#define VECTOR2_H
#include <SDL_rect.h>


class Vector2 {
public:
    double x;
    double y;

    explicit Vector2() {
        x = 0;
        y = 0;
    }

    Vector2(const double x, const double y) {
        this->x = x;
        this->y = y;
    }

    //TODO: add some math functions for Vector2 later.

    [[nodiscard]] SDL_Rect asRect() const;

    Vector2& operator+=(const Vector2& other) {

        x += other.x;
        y += other.y;

        return *this;

    }

    Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;

        return *this;
    }

    Vector2& operator*=(const Vector2& other) {
        x *= other.x;
        y *= other.y;

        return *this;
    }

    Vector2& operator*=(const double& other) {
        x *= other;
        y *= other;

        return *this;
    }

    Vector2& operator/=(const Vector2& other) {
        x /= other.x;
        y /= other.y;

        return *this;
    }

    Vector2& operator/=(const double& other) {
        x /= other;
        y /= other;

        return *this;
    }

};



#endif //VECTOR2_H
