//
// Created by shawn on 5/24/2025.
//

#ifndef RAYINFO_H
#define RAYINFO_H
#include "../Collider.h"
#include "../GameObject.h"
#include "../UiObject.h"
#include "../../Vector2.h"

//Information about a ray cast.

typedef struct {

    //The position at which the ray intersect with the object
    Vector2 positionHit;
    //The collider that was hit
    Collider* collider = nullptr;
    //The gameobject that was hit
    GameObject* gameObjectHit = nullptr;
    UiObject* uiObjectHit = nullptr;

    //Where the ray came from
    Vector2 castFrom;

} RayInfo;

#endif //RAYINFO_H
