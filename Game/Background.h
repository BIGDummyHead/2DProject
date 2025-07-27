//
// Created by shawn on 5/17/2025.
//

#ifndef BACKGROUND_H
#define BACKGROUND_H
#include "Texture.h"


class Background : public Texture {

private:
    void drawAround(Vector2 pos);

public:
    Background() {
        drawRadius = 1;
    }

    Background(SDL_Texture* loadText, const int radius) : Texture(loadText) {
        drawRadius = radius;
    }

    int drawRadius;
    void render(Vector2 where) override;
};



#endif //BACKGROUND_H
