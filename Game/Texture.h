//
// Created by shawn on 5/16/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL_render.h>

#include "../Draw.h"


class Texture {

public:


    virtual ~Texture() = default;

    SDL_Texture* loadedTexture;
    SDL_Rect renderedTexture{}; //init with an empty value

    Texture() {
        loadedTexture = nullptr;
    }

    explicit Texture(SDL_Texture* text) {
        loadedTexture = text;
    }

    virtual void render(Vector2 where);


};



#endif //TEXTURE_H
