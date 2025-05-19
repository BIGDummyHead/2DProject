//
// Created by shawn on 5/18/2025.
//

#include "Collider.h"
//
// Created by shawn on 5/18/2025.
//

#include "Collider.h"

void Collider::drawDebugCollider(const draw *drawTool) const {

    SDL_SetRenderDrawColor(drawTool->getApp().renderer, 0, 255, 0,  255);
    //top line
    drawTool->drawLine(leftTopPoint->getPosition(), rightTopPoint->getPosition());

    //left line
    drawTool->drawLine(leftTopPoint->getPosition(), leftBottomPoint->getPosition());

    //bottom line
    drawTool->drawLine(leftBottomPoint->getPosition(), rightBottomPoint->getPosition());

    //right line
    drawTool->drawLine(rightBottomPoint->getPosition(), rightTopPoint->getPosition());
}