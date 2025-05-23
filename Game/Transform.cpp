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

Vector2 Transform::getPosition() const {

    if(parentPtr == nullptr)
        return position; //absolute


   return  parentPtr->getPosition() + position;
}

void Transform::setPosition(const Vector2 &pos) {
    lastPosition = position; //update the last position
    position = pos;
}

Vector2 Transform::getRotation() const {
    return rotation;
}

void Transform::setRotation(const Vector2 &rot) {
    rotation = rot;
}

Vector2 Transform::getVelocity(const bool useLastStored) {

    if(useLastStored) //do not recalculate
        return velocity;

    velocity = (position - lastPosition).absolute();
    lastPosition = getPosition();

    return velocity;
}








