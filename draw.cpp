//
// Created by shawn on 5/16/2025.
//

#include "draw.h"
#include <iostream>
#include <SDL_log.h>
#include <SDL_render.h>
#include <SDL_surface.h>

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
SDL_Rect draw::blit(SDL_Texture *texture, const Vector2 position, const SDL_Rect* copySrc) const {
    SDL_Rect dest = position.asRect();

    SDL_QueryTexture(texture, nullptr, nullptr, &dest.w, &dest.h);

    SDL_RenderCopy(getApp().renderer, texture, copySrc, &dest);

    return dest;
}

SDL_Rect draw::blitSheet(SDL_Texture *texture, const int rows, const int columns, const int renderRow, const int renderCol, const Vector2 renderPosition, const Vector2 scalingFactor) const {

    int textWidth = 0;
    int textHeight = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);

    //based on the given texture, determine how big an object is
    const int spriteWidth = textWidth / columns;
    const int spriteHeight = textHeight / rows;

    //calculate the src rectangle
    const int srcX = spriteWidth * renderCol;
    const int srcY = spriteHeight * renderRow;
    const SDL_Rect srcRect = { srcX, srcY, spriteWidth, spriteHeight };

    //place where we should take from the texture
    SDL_Rect renderRectPosition = renderPosition.asRect();
    renderRectPosition.w = static_cast<int>(spriteWidth * scalingFactor.x);
    renderRectPosition.h = static_cast<int>(spriteHeight * scalingFactor.y);

    //center the object's render position
    renderRectPosition.x -= renderRectPosition.w / 2;
    renderRectPosition.y -= renderRectPosition.h / 2;

    // Draw a red rectangle behind the sprite
    //SDL_SetRenderDrawColor(getApp().renderer, 255, 0, 0, 255); // Red color
    //SDL_RenderFillRect(getApp().renderer, &dest);


    // Render the sprite
    SDL_RenderCopy(getApp().renderer, texture, &srcRect, &renderRectPosition);

    return renderRectPosition;
}

void draw::drawLine(const Vector2 &from, const Vector2 &to) const {

   SDL_RenderDrawLineF(getApp().renderer, from.x, from.y, to.x, to.y);
}





