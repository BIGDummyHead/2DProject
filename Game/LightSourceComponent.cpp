//
// Created by shawn on 8/1/2025.
//

#include "LightSourceComponent.h"


void LightSourceComponent::awake() {
}

void LightSourceComponent::destroy() {
}

void LightSourceComponent::start() {
}

void LightSourceComponent::setSource(LightSource *source) {
    if (this->source) {
        delete this->source;
    }

    source->setColor(255, 165, 0, 255);
    this->source = source;
    Draw::getInstance()->addLightSource(this->source);
}


void LightSourceComponent::update() {
    if (source != nullptr) {
        source->position = getAttachedObject()->transform->getPosition();
    }
}



