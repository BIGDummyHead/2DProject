//
// Created by shawn on 7/31/2025.
//

#ifndef TILETEXTURE_H
#define TILETEXTURE_H

class TileTexture final : public Texture {
private:
    SDL_Texture *texture;
    int rows;
    int cols;
    int renderRow;
    int renderCol;

    Draw *tool;

    Vector2 scale;

public:
    TileTexture(SDL_Texture *texture, const int &rows, const int &columns,
                const int &renderRow, const int &renderCol, const Vector2 &scale) {
        this->texture = texture;
        this->rows = rows;
        this->cols = columns;
        this->renderRow = renderRow;
        this->renderCol = renderCol;
        this->scale = scale;

        tool = Draw::getInstance();
    }

    void render(Vector2 where) override {
        int textWidth = 0;
        int textHeight = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);

        // Determine tile width and height based on texture size and grid
        const int spriteWidth = textWidth / cols;
        const int spriteHeight = textHeight / rows;

        // Source rectangle (portion of texture to use)
        const int srcX = spriteWidth * renderCol;
        const int srcY = spriteHeight * renderRow;
        const SDL_Rect srcRect = {srcX, srcY, spriteWidth, spriteHeight};

        // Destination rectangle (where to render on screen)
        SDL_Rect renderRectPosition = {
            static_cast<int>(where.x),
            static_cast<int>(where.y),
            static_cast<int>(spriteWidth * scale.x),
            static_cast<int>(spriteHeight * scale.y)
        };

        // Draw the tile from tilesheet
        SDL_RenderCopy(Draw::getRenderer(), texture, &srcRect, &renderRectPosition);
    }
};

#endif //TILETEXTURE_H
