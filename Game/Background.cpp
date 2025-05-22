//
// Created by shawn on 5/17/2025.
//

#include "Background.h"

void Background::drawAround(draw drawTool, Vector2 pos) {
    //TODO: Draw around the position
}


void Background::render(const draw &drawTool, Vector2 where) {
    drawTool.blit(loadedTexture, where);

    for(int i = 1; i < drawRadius; i++) {

        //TODO: modify where
        //drawAround(drawTool, where);
    }
}
