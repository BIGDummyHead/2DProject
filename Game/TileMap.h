//
// Created by shawn on 7/28/2025.
//

#ifndef TILEMAP_H
#define TILEMAP_H
#include <SDL_render.h>
#include <string>
#include "../Vector2.h"
#include <vector>
#include "../Draw.h"
#include "TileLayer.h"


class TileMap {
public:

    typedef Vector2 ROW_COLUMN;
    typedef Vector2 MAP_SIZE;

private:
    SDL_Texture *fileTexture;
    std::string file;
    MAP_SIZE size;
    int indexing = 0;
    Vector2 scale;
    std::vector<TileLayer*> createdLayers;

public:
    TileMap(const std::string &textureTileMap, const MAP_SIZE &size, const Vector2 &scale) {
        this->file = textureTileMap;
        this->fileTexture = Draw::getInstance()->loadTexture(file);

        if (!fileTexture) {
            throw std::runtime_error("Failed to load texture for tile map! " + file);
        }

        this->size = size;

        //Get the size of the texture.

        this->scale = scale;

    }

    TileLayer* createLayer(const std::string &csvFilePath, const int& renderLayer = 0);

    TileLayer* createLayer(const std::vector<std::vector<int> > &data, const int& renderLayer = 0);

    std::vector<TileLayer*> getLayers();

    std::vector<GameObject*> createAllLayers(const Vector2& position);

};




#endif //TILEMAP_H
