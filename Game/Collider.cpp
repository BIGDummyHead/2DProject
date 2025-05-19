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
    drawTool->drawLine(leftTopPoint->position, rightTopPoint);

    //left line
    drawTool->drawLine(leftTopPoint, leftBottomPoint);

    //bottom line
    drawTool->drawLine(leftBottomPoint, rightBottomPoint);

    //right line
    drawTool->drawLine(rightBottomPoint, rightTopPoint);
}
