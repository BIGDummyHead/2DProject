//
// Created by shawn on 6/5/2025.
//

#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include <SDL_render.h>

#include "Transform.h"

#endif //LIGHTSOURCE_H

class LightSource {

private:
    bool dynamic = false;

public:

    static std::vector<LightSource*> lights;

    Vector2 position{};
    double distance = 100;
    double angle = 90;
    double radius = 360;
    bool createRayCastedShadowing = true;

    Uint8 r = 255;
    Uint8 g = 255;
    Uint8 b = 255;
    Uint8 a = 200;

    bool hasDrawn = false;

    std::vector<SDL_Vertex> vertices{};
    std::vector<int> indices{};

    //increase for a smoother shadow per frame or statically.
    int rayCastCount = 200;

    void setAsDynamic() {
        dynamic = true;
    }

    void setAsStatic() {
        dynamic = false;
    }

    [[nodiscard]] bool isDynamic() const {
        return  dynamic;
    }

    [[nodiscard]] bool isStatic() const {
        return !dynamic;
    }


    LightSource(const Vector2& position, const double& distance, const double& intensity, const double& angle, const double& radius) {
        this->position = position;
        this->distance = distance;
        this->angle = angle;
        this->radius = radius;
        lights.push_back(this);
    }

    LightSource() = default;

    void setColor(const Uint32& r,const  Uint32& g, const Uint32& b, const Uint32& a = 255);

};