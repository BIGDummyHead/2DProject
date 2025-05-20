//
// Created by shawn on 5/16/2025.
//

#include "Texture.h"

void Texture::render(const draw& drawTool, Vector2 where) {
    drawTool.blit(loadedTexture, where);
}
