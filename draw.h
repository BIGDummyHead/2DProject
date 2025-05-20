//
// Created by shawn on 5/16/2025.
//

#ifndef DRAW_H
#define DRAW_H
#include "structs.h"
#include <cstdio>
#include <map>
#include <SDL_render.h>
#include <stdexcept>

#include "Vector2.h"
#include "SDL_image/include/SDL2/SDL_image.h"

//Class to help with drawing features and scene rendering
class draw {

private:
    App* drawingFor;
    std::map<std::string, SDL_Texture*> loadedTextures;

public:
    [[nodiscard]] App getApp() const;


    explicit draw(App* drawingFor) {
        this->drawingFor = drawingFor;
    }

    void prepareScene() const;
    void presentScene() const;
    [[nodiscard]] SDL_Texture* loadTexture(const std::string& filePath);

    void blit(SDL_Texture *texture, Vector2 position, const SDL_Rect* copySrc = nullptr) const;

    void blitSheet(SDL_Texture *texture, int rows, int columns, int renderRow, int renderCol, Vector2 renderPosition, Vector2 scalingFactor) const;

    void drawLine(const Vector2& from, const Vector2& to) const;
};



#endif //DRAW_H
