//
// Created by shawn on 6/14/2025.
//

#include "Component.h"
#include "GObject.h"

GObject *Component::getAttachedObject() const {
    return attachedObject;
}

void Component::setAttachedObject(GObject *obj) {
    attachedObject = obj;
}

void Component::destroy() {

}

void Component::start() {

}

void Component::update() {

}

void Component::awake() {

}



