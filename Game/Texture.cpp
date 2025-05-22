//
// Created by shawn on 5/16/2025.
//

#include "Texture.h"

void Texture::render(const draw& drawTool, Vector2 where) {
     renderedTexture = drawTool.blit(loadedTexture, where);
}
