//
// Created by shawn on 5/24/2025.
//

#ifndef RAYCASTER_H
#define RAYCASTER_H
#include "RayInfo.h"
#include "../../Vector2.h"
#include <cmath>

#include "Castable.h"
#include "Ray.h"

#ifndef RAD2DEG
#define RAD2DEG 57.29578
#endif



class Raycaster {

public:

    static bool cast(const Ray& ray, RayInfo* rayInformation);
    //Cast a ray from the mouse to get relevant information about the UI hit.
    static bool castUI(RayInfo* rayInformation);
    static bool lineIntersectsRect(const Vector2& rayStart, const Vector2& rayEnd, const Vector2& position, const Collider* collider, Vector2* intersection);
    static void drawCast(const Ray& ray, SDL_Renderer* renderer, const RayInfo& rInfo);
    static  Vector2 createEndPoint(const Ray& ray);
    static bool castFromMouse(RayInfo* rayInformation, const double& radius = 5, SDL_Renderer* shouldRender = nullptr);
};



#endif //RAYCASTER_H
