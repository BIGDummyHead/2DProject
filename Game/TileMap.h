//
// Created by shawn on 7/28/2025.
//

#ifndef TILEMAP_H
#define TILEMAP_H
#include <SDL_render.h>
#include <string>

#include "../Vector2.h"
#include <unordered_map>
#include <vector>

#include "Texture.h"
#include "../Draw.h"

class TileMap {
public:
    typedef Vector2 MAP_SIZE;
    typedef Vector2 ROW_COLUMN;

    struct TileInfo {
    public:
        ROW_COLUMN position;
        bool hasCollider;
    };

    class TileTexture final : public Texture {

    private:
        SDL_Texture* texture;
        int rows;
        int cols;
        int renderRow;
        int renderCol;

        Draw* tool;

        Vector2 scale;

    public:

        TileTexture(SDL_Texture* texture, const int& rows, const int& columns,
            const int& renderRow, const int& renderCol) {
            this->texture = texture;
            this->rows = rows;
            this->cols = columns;
            this->renderRow = renderRow;
            this->renderCol = renderCol;

            tool = Draw::getInstance();
            scale = {1,1};
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

private:
    SDL_Texture *fileTexture;
    std::string file;
    MAP_SIZE size;

    std::unordered_map<int, TileInfo> tileMap;
    int indexing = 0;

    int totalWidth;
    int totalHeight;

    int individualTileWidth;
    int individualTileHeight;

public:
    TileMap(const std::string &textureTileMap, const MAP_SIZE &size) : tileMap() {
        this->file = textureTileMap;
        this->fileTexture = Draw::getInstance()->loadTexture(file);

        if (!fileTexture) {
            throw std::runtime_error("Failed to load texture for tile map! " + file);
        }

        this->size = size;

        //Get the size of the texture.
        const int queryResult =
                SDL_QueryTexture(fileTexture, nullptr, nullptr, &totalWidth, &totalHeight);

        if (queryResult != 0) {
            throw std::runtime_error(
                "Failed to query texture for file " + file + " with result " + std::to_string(queryResult));
        }

        individualTileWidth = totalWidth / size.y;
        individualTileHeight = totalHeight / size.x;
    }

    //Add a tile and get the index in which it will lie.
    int addTile(const ROW_COLUMN &position, const bool &includeCollider);

    TileInfo getTile(const int &index);

    //Draw tiles based a CSV file.
    void createMap(Vector2 startingPosition, const std::string &csvFilePath);

    //Draw tiles based on Row and Columns definition. Recommended to use drawTile(csvFILE);
    void createMap(Vector2 startingPosition, const std::vector<std::vector<int> > &data);
};


#endif //TILEMAP_H
