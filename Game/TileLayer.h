//
// Created by shawn on 7/31/2025.
//

#ifndef TILELAYER_H
#define TILELAYER_H
#include <functional>
#include <typeindex>

#include "GameObject.h"
#include "TileTexture.h"
#include "../FileHelper.h"

template<typename T>
concept DerivesComponent = std::derived_from<T, Component>;

class TileLayer {
public:
    typedef std::function<void(int componentValue, GameObject *createObject)> ComponentCallback;

    typedef std::tuple<Vector2, int, ComponentCallback> ComponentTileInfo;

private:
    std::vector<std::vector<bool> > colliderInfo;

    std::vector<std::type_index> componentTypeInfos;

    std::unordered_map<std::type_index,
        std::vector<ComponentTileInfo> *>
    typeInfoToComponentTileInfo;

    std::vector<std::vector<int> > mapData;

    std::unordered_map<int, Vector2> tileMapper;

    Vector2 size;

    SDL_Texture* fileTexture;
    Vector2 scale;

    int totalWidth;
    int totalHeight;

    int individualTileWidth;
    int individualTileHeight;

    int renderIndex;

public:
    TileLayer(const std::vector<std::vector<int> > &csvMatrix, SDL_Texture *fileTexture,
              const Vector2 &size, const Vector2 &scale, const int& renderIndex) {

        this->fileTexture = fileTexture;
        this->mapData = csvMatrix;
        this->size = size;
        this->scale = scale;

        const int queryResult =
                SDL_QueryTexture(fileTexture, nullptr, nullptr, &totalWidth, &totalHeight);

        if (queryResult != 0) {
            throw std::runtime_error(
                "Failed to query texture for file with result: " + std::to_string(queryResult));
        }

        individualTileWidth = totalWidth / size.y;
        individualTileHeight = totalHeight / size.x;

        this->renderIndex = renderIndex;

        generateTiles();
    }

    void generateTiles();
    Vector2 getTileTexturePosition(const int &index);

    //Add component info to the layer information, you must handle how the component gets added.
    template<typename I, DerivesComponent C>
    void addComponentInfo(std::vector<std::vector<I> > csvMatrix, const ComponentCallback &callbackHandle)
        requires std::is_integral_v<I>;


    //Add component info to the layer information based on a CSV file, you must handle how the component gets added.
    template<typename I, DerivesComponent C>
    void addComponentInfo(const std::string &csvFile, const ComponentCallback &callbackHandle) requires
        std::is_integral_v<I>;


    //Add collider info to the layer, this will add a collider to the GameObject* automatically if boolean results in true.
    void addColliderInfo(std::vector<std::vector<bool> > csvMatrix);

    //Add collider info to the layer based on a CSV file, this will add a collider to the GameObject* automatically if boolean results in true.
    void addColliderInfo(const std::string &csvFile);



    std::vector<GameObject *> create(const Vector2& startingPosition);
};

template<typename I, DerivesComponent C>
void TileLayer::addComponentInfo(const std::string &csvFile, const ComponentCallback &callbackHandle) requires
    std::is_integral_v<I> {
    const auto csvMatrix = FileHelper::generateCsvMatrix<I>(csvFile);

    addComponentInfo<I, C>(csvMatrix, callbackHandle);
}

template<typename I, DerivesComponent C>
void TileLayer::addComponentInfo(std::vector<std::vector<I> > csvMatrix,
                                 const ComponentCallback &callbackHandle) requires std::is_integral_v<I> {
    for (int r = 0; r < csvMatrix.size(); r++) {
        for (int c = 0; c < csvMatrix[r].size(); c++) {
            const I value = csvMatrix[r][c];

            if (!typeInfoToComponentTileInfo.contains(typeid(C))) {
                typeInfoToComponentTileInfo[typeid(C)] = new std::vector<ComponentTileInfo>();
            }

            typeInfoToComponentTileInfo[typeid(C)]->push_back({
                Vector2{static_cast<double>(r), static_cast<double>(c)}, value, callbackHandle
            });

            const auto item = std::ranges::find(componentTypeInfos, typeid(C));
            if (item == componentTypeInfos.end()) {
                componentTypeInfos.push_back(typeid(C));
            }
        }
    }
}


#endif //TILELAYER_H
