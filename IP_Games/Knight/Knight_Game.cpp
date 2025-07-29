//
// Created by shawn on 7/1/2025.
//

#include "Knight_Game.h"

#include "Player.h"
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
    thePlayer->collider = new Collider(30, 30, false);
}
