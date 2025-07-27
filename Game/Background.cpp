//
// Created by shawn on 5/17/2025.
//

#include "Background.h"

void Background::drawAround(Vector2 pos) {
    //TODO: Draw around the position
}


void Background::render(Vector2 where) {
    Draw::getInstance()->blit(loadedTexture, where);

    for(int i = 1; i < drawRadius; i++) {

        //TODO: modify where
        //drawAround(drawTool, where);
    }
}
