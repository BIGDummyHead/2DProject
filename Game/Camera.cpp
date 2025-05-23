//
// Created by shawn on 5/19/2025.
//

#include "Camera.h"
#include <iostream>
#include "../defs.h"

Camera* Camera::mainCamera;

bool Camera::isInRenderView(const Vector2& other) const {
    const Vector2 currentPosition = transform->getPosition();




    const double xDistance = std::abs(currentPosition.x)  - std::abs(other.x);
    const double yDistance = std::abs(currentPosition.y) - std::abs(other.y);


    return xDistance <= renderFOV.x / 2 && yDistance <= renderFOV.y / 2;
}

