//
// Created by shawn on 5/18/2025.
//

#ifndef COLLIDER_H
#define COLLIDER_H
#include "Sheet.h"
#include "Texture.h"
#include "Transform.h"
#include "../draw.h"
#include "../Vector2.h"
#include <cmath>


class Collider {

public:
    double width;
    double height;
    Vector2 center;
    bool isStatic = true;

    double mass = 1;

    //Create a default collider
    Collider(const double& width, const double& height, const bool isStatic = true) {
        this->width = fabs( width );
        this->height = fabs( height );
        this->isStatic = isStatic;
    }

    //Create a collider from a SDL_Rect object
    explicit Collider(const SDL_Rect& from, const bool isStatic = true) : Collider(from.w, from.h, isStatic) {

    }

    //Copy over an existing collider
    Collider(const Collider& ref) : Collider(ref.width, ref.height, ref.isStatic) {

    }

    //Check if this collider is colliding with another collider
    [[nodiscard]] bool isColliding(const Collider& other, Vector2& push) const;

    [[nodiscard]] Vector2 getSize() const;

    void drawColliderBox(SDL_Renderer* renderer, const Vector2& drawnAt) const;

    [[nodiscard]]Vector2 getTopLeft() const;
    [[nodiscard]]Vector2 getTopRight() const;
    [[nodiscard]]Vector2 getBottomLeft() const;
    [[nodiscard]]Vector2 getBottomRight() const;

    void pushOut(Collider* other, Transform* thisColliderTransform, Transform* otherTransform);

    static Vector2 createBoxFromSheet(const Sheet& sheet);
    static  Vector2 createBoxFromTexture(const Texture& text);

};


#endif //COLLIDER_H