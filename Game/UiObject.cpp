//
// Created by shawn on 6/14/2025.
//

#include "UiObject.h"

std::vector<UiObject*> UiObject::registeredUI;

void UiObject::render() {
    //virtual class
    this->drawingTexture->render(*drawTool, position);
}

void UiObject::onMouseClick() {

}

void UiObject::onMouseExit() {

}

void UiObject::onMouseRelease() {

}

void UiObject::whileMouseDown() {

}

void UiObject::whileMouseOver() {

}

void UiObject::onMouseEnter() {

}

void UiObject::whileMouseOut() {

}


void UiObject::setMouseIn() {

    if(!mouseFocus) {
        //in case this function is called more than once
        onMouseEnter();
        mouseFocus = true;
    }

}

void UiObject::setMouseOut() {

    if(mouseFocus) {
        //in case this function is called more than once
        onMouseExit();
        mouseFocus = false;
    }

}

bool UiObject::isMouseFocused() const {
    return mouseFocus;
}

std::vector<UiObject*> UiObject::getRegisteredUI() {
    return registeredUI;
}










