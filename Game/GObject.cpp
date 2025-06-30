//
// Created by shawn on 5/16/2025.
//

#include "GObject.h"

std::unordered_set<GObject*> GObject::registeredObjects;
std::unordered_set<GObject*> GObject::activeObjects;
std::unordered_set<GObject*> GObject::inactiveObjects;

bool GObject::getIsActive() const {
    return isActive;
}

void GObject::update() {
}

void GObject::onRender(const Vector2& drawnAt) {

}

void GObject::onCollision(Collider *other) {

}

void GObject::destroy() {
    setIsActive(false);
    delete collider;
    delete transform;
    inactiveObjects.erase(this);
    //registeredObjects.erase(this);
}

void GObject::updateFrame() {

    for(const auto comp : activeComponents) {

        if(!comp->hasStarted) {
            comp->start();
            comp->hasStarted = true;
            continue;
        }

        comp->update();
    }

    //do internal things
    update();
}







void GObject::setIsActive(const bool status)  {
    if(isActive && status || !isActive && !status) { //there is nothing tto do
        return;
    }

    isActive = status;

    if(isActive) {

        const auto found = inactiveObjects.find(this);

        if(found != inactiveObjects.end()) {
            inactiveObjects.erase(*found);
        }

        activeObjects.insert(*found);

    }
    else {
        const auto found = activeObjects.find(this);

        if(found != activeObjects.end()) {
            activeObjects.erase(*found);
        }

        inactiveObjects.insert(*found);
    }
}







