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







