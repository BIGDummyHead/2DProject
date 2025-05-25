//
// Created by shawn on 5/25/2025.
//

#ifndef RAY_H
#define RAY_H
#include <float.h>

#include "../../Vector2.h"

struct Ray {
    const Vector2& position;
    const Vector2& angle;
    const double& distance;

    Ray(const Vector2& pos, const Vector2& ang, const double& dist) : position(pos), angle(ang), distance(dist) {

        if(dist == DBL_MAX)
            SDL_LogWarn(SDL_LOG_PRIORITY_WARN, "Distance for Ray is close to DBL_MAX (Double Max) this could case unexpected consequences.");
    }
};
#endif //RAY_H
