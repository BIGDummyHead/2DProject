//
// Created by shawn on 5/24/2025.
//

#ifndef RAYCASTER_H
#define RAYCASTER_H
#include "RayInfo.h"
#include "../../Vector2.h"
#include <cmath>

#ifndef RAD2DEG
#define RAD2DEG 57.29578
#endif



class Raycaster {

public:

    static bool cast(const Vector2& position, const Vector2& angle, const double& distance, RayInfo* rayInformation);
    static bool lineIntersectsRect(const Vector2& rayStart, const Vector2& rayEnd, const GObject* obj);
};



#endif //RAYCASTER_H
