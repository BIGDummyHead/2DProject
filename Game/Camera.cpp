//
// Created by shawn on 5/19/2025.
//

#include "Camera.h"
#include "GameObject.h"

Camera* Camera::mainCamera;

bool Camera::isInRenderView(const Vector2& other) const {

    //TODO: implement a fix for when moving to the right side of the screen! It takes twice as long to return false when moving for some reason
    const int width = static_cast<int>(renderFOV.x);
    const int height = static_cast<int>(renderFOV.y);

    const Vector2 currentPosition = transform->getPosition();
    const SDL_Rect camRect = { static_cast<int>( currentPosition.x ), static_cast<int>(currentPosition.y), width, height };

    const SDL_Rect objectRect = { static_cast<int>( other.x), static_cast<int>(other.y), width, height }; //set default 5, 5 width and height just to give it something

    return SDL_HasIntersection(&camRect, &objectRect);
}

