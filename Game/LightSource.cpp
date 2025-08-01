//
// Created by shawn on 6/6/2025.
//

#include "LightSource.h"

std::vector<LightSource*> LightSource::lights; // Definition

void LightSource::setColor(const Uint32& r,const  Uint32& g, const Uint32& b, const Uint32& a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}
