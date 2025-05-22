//
// Created by shawn on 5/19/2025.
//

#ifndef CAMERA_H
#define CAMERA_H
#include <SDL_render.h>

#include "Transform.h"
#include "../Vector2.h"


class Camera {
public:
    Vector2 renderFOV;
    Transform* transform;


    Camera(const Vector2 fov, const Vector2 position) {
        transform = new Transform();
        transform->setPosition(position);
        renderFOV = fov;
    }

    [[nodiscard]] bool isInRenderView(const Vector2& other) const;

};


#endif //CAMERA_H
