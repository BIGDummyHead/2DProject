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

    static Vector2 one() {
        const Vector2 v(1,1);
        return v;
    }

    static  Vector2 zero() {
        const Vector2 v(0,0);
        return v;
    }

    //TODO: add some math functions for Vector2 later.

    [[nodiscard]] double distance(const Vector2& other) const;
    [[nodiscard]] SDL_Rect asRect() const;

    Vector2& operator+=(const Vector2& other) {

        x += other.x;
        y += other.y;

        return *this;

    }

    Vector2 operator+(const Vector2& other) const {
        const Vector2 v(x + other.x, y + other.y);
        return v;
    }

    Vector2 operator-(const Vector2& other) const {
        const Vector2 v(x-other.x, y - other.y);
        return v;
    }

    Vector2 operator/(const Vector2& other) const {

        if(other.y == 0 || other.x == 0) {
            const Vector2 badV;
            return badV;
        }

        const Vector2 v(x / other.x, y / other.y);
        return v;
    }

    Vector2 operator*(const Vector2& other) const {
        const Vector2 v(x * other.x, y * other.y);
        return v;
    }

    Vector2 operator*(const double& other) const {
        const Vector2 v(x * other, y * other);
        return v;
    }

    Vector2 operator/(const double& other) const {
        const Vector2 v(x / other, y / other);
        return v;
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

        Vector2 divisor = other;
        if(other.x == 0 || other.y == 0) {
            divisor = Vector2(1,1);
        }

        x /= divisor.x;
        y /= divisor.y;

        return *this;
    }

    Vector2& operator/=(const double& other) {
        x /= other;
        y /= other;

        return *this;
    }

};



#endif //VECTOR2_H
