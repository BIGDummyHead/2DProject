//
// Created by shawn on 7/28/2025.
//

#include "TileMap.h"
#include "GameObject.h"
#include "../FileHelper.h"


TileLayer* TileMap::createLayer(const std::string &csvFilePath, const int& renderLayer) {
    const auto csvMatrix = FileHelper::generateCsvMatrix<int>(csvFilePath);

    return createLayer(csvMatrix, renderLayer);
}

TileLayer* TileMap::createLayer(const std::vector<std::vector<int> > &data, const int& renderLayer) {
    auto* layer = new TileLayer(data, fileTexture, size, scale, renderLayer);
    createdLayers.push_back(layer);
    return layer;
}

std::vector<TileLayer *> TileMap::getLayers() {
    return createdLayers;
}

std::vector<GameObject *> TileMap::createAllLayers(const Vector2& position) {
    std::vector<GameObject*> layerObjects;

    for(const auto& layer : createdLayers) {

        auto objects = layer->create(position);
        for(auto* obj : objects) {
            layerObjects.push_back(obj);
        }

    }

    return layerObjects;
}




