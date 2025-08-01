//
// Created by shawn on 7/1/2025.
//

#include "Knight_Game.h"

#include "Player.h"
#include "../../Game/LightSourceComponent.h"
#include "../../Game/Scene.h"

Knight_Game* Knight_Game::instance;

void Knight_Game::setInstance(Knight_Game *kGame) {

    if(!instance || kGame == nullptr)
        return;

    instance = kGame;
}



//Represents building the main menu
void Knight_Game::onSceneLoad(SceneInformation sceneInfo) {
    //Load In Assets:
    //-Textures-
    //-Fonts-
    //-Audio (future)-

    const auto dimensions = App::getInstance()->getSettings()->windowDimensions;


    const Vector2 center(dimensions.x / 2, dimensions.y / 2);

    const Vector2 fov{ dimensions.x + 150, dimensions.y + 250};
    camera = new Camera(fov, Vector2{0,0});

//    auto *playerObject = new Test_Player(sceneInfo.drawingTool, center);
    thePlayer = new Player(center);
    thePlayer->setRenderLayer(1000);
    auto* pCollider = new Collider(10, 15, false);

    pCollider->offset -= {10,15};
    thePlayer->collider = pCollider;
    createMap();
}


void Knight_Game::createMap() {
    const std::string baseFile = "TileMap_Dungeon/";
    map = new TileMap(baseFile + "dungeon_tileset.png", {10, 10}, {2,2});

    const auto layer = map->createLayer(baseFile + "tiled_Dungeon.csv");
    layer->addColliderInfo(baseFile + "tiled_Collider.csv");

    auto lighting = map->createLayer(baseFile + "tiled_Lights.csv");
    lighting->addComponentInfo<int, LightSourceComponent>(baseFile + "tiled_Lights.csv", [](int value, GameObject* obj) {
        auto* source = new LightSource(obj->transform->getPosition(), 150, 100, 0, 360);
        source->createRayCastedShadowing = false;
        source->setAsDynamic();

        if(!obj->hasComponent<LightSourceComponent>())
            obj->addComponent<LightSourceComponent>()->setSource(source);

    });

    map->createLayer(baseFile + "tiled_Doors.csv");
    map->createLayer(baseFile + "tiled_Chest.csv");
    map->createLayer(baseFile + "tiled_Accessories.csv");

    layer->create({0,0});

    map->createAllLayers({0,0});

}

