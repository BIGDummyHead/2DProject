//
// Created by shawn on 5/16/2025.
//

#include "Texture.h"

void Texture::render(const Vector2 where) {
     renderedTexture = Draw::getInstance()->blit(loadedTexture, where);
}
