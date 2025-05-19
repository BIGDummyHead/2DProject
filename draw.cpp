//
// Created by shawn on 5/16/2025.
//

#include "draw.h"
#include <iostream>

App draw::getApp()  const {

    if(drawingFor == nullptr) {
        throw std::runtime_error("During draw, app was found to be a null pointer.");
    }

    return *drawingFor;
}

void draw::prepareScene() const {
    SDL_SetRenderDrawColor(getApp().renderer, 96,128,255,255);
    SDL_RenderClear(getApp().renderer);

}

void draw::presentScene() const {
    SDL_RenderPresent(getApp().renderer);
}

SDL_Texture* draw::loadTexture(const std::string& filePath) {

    if (loadedTextures.contains(filePath)) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "%s was already loaded.", filePath.c_str());
        return loadedTextures.at(filePath);
    }

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filePath.c_str());

    SDL_Texture* texture = IMG_LoadTexture(getApp().renderer, filePath.c_str());
    if (!texture) {
        throw std::runtime_error("Failed to load texture: " + filePath);
    }

    loadedTextures[filePath] = texture;

    return texture;
}

//Draws a texture at a given position. Set onlyCopy as a position from the texture to copy.
void draw::blit(SDL_Texture *texture, const Vector2 position, const SDL_Rect* copySrc) const {
    SDL_Rect dest = position.asRect();

    SDL_QueryTexture(texture, nullptr, nullptr, &dest.w, &dest.h);

    SDL_RenderCopy(getApp().renderer, texture, copySrc, &dest);
}

void draw::blitSheet(SDL_Texture *texture, const int rows, const int columns, const int renderRow, const int renderCol, const Vector2 renderPosition, const Vector2 scaleFactor) const {

    int textWidth = 0;
    int textHeight = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);

    //auto determine how much each section cost
    const int spriteWidth = textWidth / columns;
    const int spriteHeight = textHeight / rows;

    //determine where to render.
     const int srcX = spriteWidth * renderCol;
     const int srcY = spriteHeight * renderRow;


    //now we have a part of our sprite to render
    const SDL_Rect srcRect = { srcX, srcY, spriteWidth, spriteHeight };

    SDL_Rect dest = renderPosition.asRect();
    dest.w = static_cast<int>(spriteWidth * scaleFactor.x);
    dest.h = static_cast<int>(spriteHeight * scaleFactor.y);

    SDL_RenderCopy(getApp().renderer, texture, &srcRect, &dest);

}

void draw::drawLine(const Vector2 &from, const Vector2 &to) const {

    std::cout << SDL_RenderDrawLine(getApp().renderer, from.x, from.y, to.x, to.y) << std::endl;
}




