//
// Created by shawn on 6/14/2025.
//

#include "Component.h"
#include "GameObject.h"

GameObject *Component::getAttachedObject() const {
    return attachedObject;
}

void Component::setAttachedObject(GameObject *obj) {
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



