//
// Created by shawn on 7/31/2025.
//

#include "TileLayer.h"

void TileLayer::addColliderInfo(const std::string &csvFile) {
    const auto csvMatrix = FileHelper::generateCsvMatrix<bool>(csvFile);

    addColliderInfo(csvMatrix);
}

void TileLayer::addColliderInfo(std::vector<std::vector<bool> > csvMatrix) {
    if (!colliderInfo.empty()) {
        throw std::runtime_error("You cannot set the collider info of your tilemap more than once.");
    }

    colliderInfo = csvMatrix;
}

void TileLayer::generateTiles() {
    int key = 0;
    for (double r = 0; r < size.x; r++) {
        for (double c = 0; c < size.y; c++) {
            if (tileMapper.contains(key)) {
                throw std::runtime_error("This key already exist in tile mapper");
            }

            tileMapper[key] = {r, c};
            key++;
        }
    }
}

Vector2 TileLayer::getTileTexturePosition(const int &index) {
    if (!tileMapper.contains(index)) {
        throw std::runtime_error("Failure to acquire tile, OOB");
    }

    return tileMapper[index];
}

std::vector< GameObject *> TileLayer::create(const Vector2& startingPosition) {

    if(created)
        return {};

    //generate fake collider info with none
    /*if (colliderInfo.empty()) {

        //create a new collider info of the same size as the map.
        colliderInfo = std::vector<std::vector<bool>>(mapData.size());

        //for each row of get the columns
        for(int i = 0; i < mapData.size(); i++) {

            //get the size
            const size_t size = mapData[i].size();

            //set all to default value of false.
            colliderInfo[i] = std::vector(size, false);
        }

    }*/

    //choose the starting position
    Vector2 position = startingPosition;

    //containers for holding gameobjects.
    std::vector<GameObject *> tilesCreated;
    auto *mapToTile = new std::unordered_map<Vector2, GameObject *>();

    //loop over entire map
    for (size_t r = 0; r < mapData.size(); r++) {

        for (size_t c = 0; c < mapData[r].size(); c++) {

            //get the tile for rendering
            const int tile = mapData[r][c];

            //Ensure non-invalid tile, this can happen when a tile is empty.
            if (tile >= 0) {

                //Get where the tile will be positioned in the renderer
                const Vector2 texturePosition = getTileTexturePosition(tile);

                //generate a new texture for this specific tile
                auto *tileTexture = new TileTexture(
                    fileTexture,
                    size.x,
                    size.y,
                    texturePosition.x,
                    texturePosition.y,
                    scale
                );


                //Create new gameobject with generic settings, set texture, and position
                auto *mapObj = new GameObject("TileMap_Layer_" + renderIndex, renderIndex);
                mapObj->texture = tileTexture;
                mapObj->transform->setPosition(position);

                //Determine if we should generate a collider
                const bool hasCollider = colliderInfo.empty() ? false : colliderInfo[r][c];
                if (hasCollider) {
                    auto *collider = new Collider(individualTileHeight / 2, individualTileWidth / 2, scale, true);
                    mapObj->collider = collider;
                }

                //Add information here.
                tilesCreated.push_back(mapObj);
                (*mapToTile)[Vector2{static_cast<double>(r), static_cast<double>(c)}] = mapObj;
            }

            //push tile in direction right
            position.x += individualTileWidth * scale.x;
        }

        //start x over, move down
        position.x = startingPosition.x;
        position.y += individualTileHeight * scale.y;
    }

    //Component callback handler
    for (const auto &componentID: componentTypeInfos) {

        const auto callbacks = *typeInfoToComponentTileInfo[componentID];

        //loop over vector of tuple information
        for (const auto &[tilePosition, matrixTileValue, componentHandlerCallback]: callbacks) {

            //if a valid value for the tile and a callback exist
            if (matrixTileValue >= 0 && componentHandlerCallback) {
                auto *mapObj = (*mapToTile)[tilePosition];

                if (mapObj) //allow for this handler to deal with it
                    componentHandlerCallback(matrixTileValue, mapObj);
            }
        }

    }

    //get rid of map to tile, not needed anymore.
    delete mapToTile;

    created = true;

    return tilesCreated;
}

