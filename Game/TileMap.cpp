//
// Created by shawn on 7/28/2025.
//

#include "TileMap.h"

#include <fstream>

#include "GameObject.h"


TileMap::TileInfo TileMap::getTile(const int &index) {
    if (index < 0 || index > indexing - 1) {
        throw std::runtime_error("Tile does not exist at index");
    }

    return tileMap[index];
}

int TileMap::addTile(const ROW_COLUMN &position, const bool &includeCollider) {
    if (position.x < 0 || position.y < 0) {
        throw std::runtime_error("X or Y may not be null when adding a tile");
    }

    tileMap[indexing] = TileInfo(position, includeCollider);
    indexing++;
    return indexing - 1;
}

void TileMap::createMap(Vector2 startingPosition, const std::string &csvFilePath) {
    std::vector<std::vector<int> > tileMapVector;

    std::ifstream csvFile(csvFilePath);
    std::string line;

    while (std::getline(csvFile, line)) {
        std::stringstream ss(line);
        std::string tile;
        std::vector<int> row;

        while (std::getline(ss, tile, ',')) {
            row.push_back(std::stoi(tile));
        }

        tileMapVector.push_back(row);
    }

    createMap(startingPosition, tileMapVector);
}


void TileMap::createMap(Vector2 startingPosition, const std::vector<std::vector<int> > &data) {

    const float repeatX = startingPosition.x;

    for (size_t r = 0; r < data.size(); r++) {

        for (size_t c = 0; c < data[r].size(); c++) {
            const int tile = data[r][c];

            auto [position, hasCollider] = getTile(tile);

            const auto tilePosition = position;
            auto *tileTexture = new TileTexture(
                fileTexture,
                size.x,
                size.y,
                tilePosition.x,
                tilePosition.y
            );

            auto *mapObj = new GameObject("tile_map_item", 0);
            mapObj->texture = tileTexture;

            mapObj->transform->setPosition(startingPosition);

            if (hasCollider) {
                auto* collider = new Collider(individualTileHeight / 2, individualTileWidth / 2, true);

                mapObj->collider = collider;
            }

            startingPosition.x += individualTileWidth;
        }

        startingPosition.x = repeatX;
        startingPosition.y += individualTileHeight;
    }
}


