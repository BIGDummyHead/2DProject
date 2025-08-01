//
// Created by shawn on 5/16/2025.
//

#ifndef VECTOR2_H
#define VECTOR2_H
#include <SDL_rect.h>
#include <iostream>
#include <sstream>
#include <cmath>

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


    bool operator==(const Vector2& other) const {
        return std::abs(x - other.x) < 0.001f &&
               std::abs(y - other.y) < 0.001f;
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


    [[nodiscard]] Vector2 normalize() const;


    [[nodiscard]] std::string toString() const {

        std::stringstream ss;
        ss << "(" << x << "," << y << ")";
        return ss.str();
    }

    [[nodiscard]] double magnitude() const;

    [[nodiscard]] Vector2 absolute() const;

    [[nodiscard]]Vector2 normalized() const;

    [[nodiscard]]double length() const;

};

namespace std {
    template<>
    struct hash<Vector2> {
        std::size_t operator()(const Vector2& v) const noexcept {
            auto hx = std::hash<int>{}(static_cast<int>(v.x * 1000));
            auto hy = std::hash<int>{}(static_cast<int>(v.y * 1000));
            return hx ^ (hy << 1);
        }
    };
}


#endif //VECTOR2_H
