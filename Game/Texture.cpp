//
// Created by shawn on 5/16/2025.
//

#include "Texture.h"

SDL_Rect Texture::render(const draw& drawTool, Vector2 where) {
    return drawTool.blit(loadedTexture, where);
}
