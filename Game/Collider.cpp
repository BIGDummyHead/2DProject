//
// Created by shawn on 5/18/2025.
//

#include "Collider.h"
//
// Created by shawn on 5/18/2025.
//

#include <cmath>

#include "Collider.h"

#include <algorithm>

//Ensure that sheet.renderedTexture is set before calling this function
Vector2 Collider::createBoxFromSheet(const Sheet &sheet) {

    //produces 40
    //std::cout << textureSheet.renderedTexture.w / textureSheet.totalCols() / 2 << std::endl;

    //produces 25, width
    //std::cout << textureSheet.renderedTexture.y / textureSheet.totalRows() / 2 << std::endl;

    const int x = sheet.renderedTexture.y / sheet.totalRows() / 2;
    const int y = sheet.renderedTexture.w / sheet.totalCols() / 2;

    const Vector2 v (x, y);

    return v;
}

Vector2 Collider::createBoxFromTexture(const Texture &text) {
    const int x = text.renderedTexture.y / 2;
    const int y = text.renderedTexture.x / 2;
    const Vector2 v(x,y);
    return v;
}


bool Collider::isColliding(const Collider& other, Vector2& push) const {
    // Get the bounds of this collider
    const Vector2 thisTopLeft = getTopLeft();
    const Vector2 thisBottomRight = getBottomRight();

    // Get the bounds of the other collider
    const Vector2 otherTopLeft = other.getTopLeft();
    const Vector2 otherBottomRight = other.getBottomRight();

    // Check for overlap in both axes
    const bool hasCollision =
            thisTopLeft.x < otherBottomRight.x &&
            thisBottomRight.x > otherTopLeft.x &&
            thisTopLeft.y < otherBottomRight.y &&
            thisBottomRight.y > otherTopLeft.y;
    if(hasCollision) {

        //1st algo coming from the top
        //2nd algo coming from the bottom
        //when coming from the top do the following: thisBottomRight.y - otherTopLeft.y
        //when coming from the bottom do the following: otherBottomRight.y - thisTopLeft.y
        push.y = center.y < other.center.y ? (thisBottomRight.y - otherTopLeft.y) * -1: (otherBottomRight.y - thisTopLeft.y);

        //algorithm to get the pushforce for X as well
        push.x = center.x > other.center.x ? otherBottomRight.x - thisTopLeft.x : otherTopLeft.x - thisBottomRight.x;

        // test which type of collision horizontal or vertical
        if (fabs(push.x) > fabs(push.y)) {
            // vertical collision, do nothing on the X axis
            push.x = 0;
        } else {
            // horizontal collision, do nothing on the Y axis
            push.y = 0;
        }

    }

    return hasCollision;
}

Vector2 Collider::getBottomLeft() const {
    return Vector2{center.x - width, center.y + height};
}

Vector2 Collider::getBottomRight() const {
    return Vector2{center.x + width, center.y + height};
}

Vector2 Collider::getTopLeft() const {
    return Vector2{center.x - width, center.y - height};
}

Vector2 Collider::getTopRight() const {
    return Vector2{center.x + width, center.y - height};
}


Vector2 Collider::getSize() const {
    return Vector2{width, height};
}

void Collider::drawColliderBox() const {
    // Use your bounding box directly instead of drawnAt

    const Vector2 topLeft = getTopLeft();
    const Vector2 bottomRight = getBottomRight();

    SDL_Rect rect;
    rect.x = static_cast<int>(topLeft.x);
    rect.y = static_cast<int>(topLeft.y);
    rect.w = static_cast<int>(bottomRight.x - topLeft.x);
    rect.h = static_cast<int>(bottomRight.y - topLeft.y);

    auto* renderer = Draw::getRenderer();
    // Set draw color (green)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_RenderDrawRect(renderer, &rect);
}

