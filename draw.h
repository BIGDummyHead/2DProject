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
#include <vector>

#include "Vector2.h"
#include "Game/LightSource.h"
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

    SDL_Rect blit(SDL_Texture *texture, Vector2 position, const SDL_Rect* copySrc = nullptr) const;

    SDL_Rect blitSheet(SDL_Texture *texture, int rows, int columns, int renderRow, int renderCol, Vector2 renderPosition, Vector2 scalingFactor) const;

    void drawLine(const Vector2& from, const Vector2& to) const;

    void light(const Vector2& position, const double& radius);

    void drawCircle(const Vector2& position, const int& radius);

    SDL_Texture* createLightMap(std::vector<LightSource> &lights);
    void drawGradientLine(Vector2 start, Vector2 end, double totalDistance, Uint8 r, Uint8 g, Uint8 b, double intensity) const;
};



#endif //DRAW_H
