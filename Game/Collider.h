//
// Created by shawn on 5/18/2025.
//

#ifndef COLLIDER_H
#define COLLIDER_H
#include "Transform.h"
#include "../draw.h"
#include "../Vector2.h"


class Collider {

public:
    Transform* leftTopPoint;
    Transform* rightTopPoint;
    Transform* leftBottomPoint;
    Transform* rightBottomPoint;

    //Create a default collider
    Collider(Transform* attached, const float& leftX, const float& bottomY, const float& rightX, const float& topY) {

        leftBottomPoint = new Transform();
        leftBottomPoint->setParent(attached);
        leftBottomPoint->setPosition(Vector2(leftX, bottomY));

        leftTopPoint = new Transform();
        leftTopPoint->setParent(attached);
        leftTopPoint->setPosition(Vector2(leftX, topY));

        rightBottomPoint = new Transform();
        rightBottomPoint->setParent(attached);
        rightBottomPoint->setPosition(Vector2(rightX, bottomY));

        rightTopPoint = new Transform();
        rightTopPoint->setParent(attached);
        rightTopPoint->setPosition(Vector2(rightX, topY));
    }

    //Check if this collider is colliding with another collider
    bool isColliding(const Collider& other);

    //Draw this collider
    void drawDebugCollider(const draw* drawTool) const;

};


#endif //COLLIDER_H
