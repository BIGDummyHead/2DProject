//
// Created by shawn on 5/16/2025.
//

#include "Transform.h"

Transform *Transform::getParent() const {
    return  parentPtr;
}

void Transform::setParent(Transform *parent) {

    if(parent == nullptr) {
        return;
    }else if(parentPtr == parent)
        return; //already parent
    else if(parentPtr == nullptr && parentPtr != parent) {
        //parentPtr is changing
        //TODO: implement removal of child from the other parent.
    }

    parentPtr = parent;
    parentPtr->children.push_back(this);
}

Vector2 Transform::getPosition() {

    if(parentPtr == nullptr)
        return position; //absolute

    return  parentPtr->getPosition() + position;
}


void Transform::setPosition(const Vector2 &pos) {
    position = pos;
}

Vector2 Transform::getRotation() const {
    return rotation;
}

void Transform::setRotation(const Vector2 &rot) {
    rotation = rot;
}



