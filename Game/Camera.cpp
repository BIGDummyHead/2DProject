//
// Created by shawn on 5/19/2025.
//

#include "Camera.h"
#include <iostream>
#include "../defs.h"

bool Camera::isInRenderView(const Transform *other) const {
    //I believe we need to take the current transform position that we have and we need to measure the distance

    //we divide this by two to get the real FOV

    const double distance = transform->getPosition().distance(other->getPosition());

    return distance <= renderFOV.x && distance <= renderFOV.y;
}

SDL_Rect Camera::getPosition(const Vector2 &position) {

    SDL_Rect renderPosition {
        static_cast<int>( position.x ), static_cast<int>(position.y),SCREEN_WIDTH,SCREEN_HEIGHT
    };

    return renderPosition;
}

