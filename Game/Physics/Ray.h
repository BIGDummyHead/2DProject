//
// Created by shawn on 5/25/2025.
//

#ifndef RAY_H
#define RAY_H
#include <float.h>

#include "../../Vector2.h"

struct Ray {
    const Vector2& position;
    Vector2 angle;
    const double& distance;

    static Vector2 angleToVector(const double& degree) {
        const double radian = degree * M_PI / 180.0;
        return Vector2{cos(radian), sin(radian)};
    }

    static double vectorToAngle(const Vector2& degree) {
        const double radians = atan2(degree.y, degree.x);
        return radians * 180.0 / M_PI;
    }

    [[nodiscard]] double getVectorAsDegree() const {
        return vectorToAngle(angle);
    }

    //Take an Angle (in degrees) and set the Vector angle
    void setAngleFromDegree(const double& degree) {
        angle = angleToVector(degree);
    }

    Ray(const Vector2& pos, const Vector2& ang, const double& dist) : position(pos), angle(ang), distance(dist) {

        if(dist == DBL_MAX)
            SDL_LogWarn(SDL_LOG_PRIORITY_WARN, "Distance for Ray is close to DBL_MAX (Double Max) this could case unexpected consequences.");
    }

    Ray(const Vector2& pos, const double& ang, const double& dist) : position(pos), distance(dist) {
        angle = angleToVector(ang);
    }
};
#endif //RAY_H
