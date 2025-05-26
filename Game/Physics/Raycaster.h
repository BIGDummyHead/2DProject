//
// Created by shawn on 5/24/2025.
//

#ifndef RAYCASTER_H
#define RAYCASTER_H
#include "RayInfo.h"
#include "../../Vector2.h"
#include <cmath>

#include "Ray.h"

#ifndef RAD2DEG
#define RAD2DEG 57.29578
#endif



class Raycaster {

public:

    static bool cast(const Ray& ray, RayInfo* rayInformation);
    static bool lineIntersectsRect(const Vector2& rayStart, const Vector2& rayEnd, const GObject* obj, Vector2* intersection);
    static void drawCast(const Ray& ray, SDL_Renderer* renderer, const RayInfo& rInfo);
    static  Vector2 createEndPoint(const Ray& ray);
};



#endif //RAYCASTER_H
