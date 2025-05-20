//
// Created by shawn on 5/16/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL_render.h>

#include "../draw.h"


class Texture {
public:
    virtual ~Texture() = default;

    SDL_Texture* loadedTexture;

    Texture() {
        loadedTexture = nullptr;
    }

    explicit Texture(SDL_Texture* text) {
        loadedTexture = text;
    }

    virtual SDL_Rect render(const draw &drawTool, Vector2 where);
};



#endif //TEXTURE_H
