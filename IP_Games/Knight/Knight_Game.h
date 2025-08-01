//
// Created by shawn on 7/1/2025.
//

#ifndef KNIGHT_GAME_H
#define KNIGHT_GAME_H
#include "Player.h"
#include "../../Game/Scene.h"
#include "../../Game/TileMap.h"
#include "../../Settings/Parser.h"

//Knight Game manager and controller.
class Knight_Game : public Scene {

private:
    bool isDebug = true;
    std::string SETTINGS_PATH = "assets/Settings/settings.par";
    std::unordered_map<std::string, std::string> settings;
    static Knight_Game* instance;


public:

    Player* thePlayer = nullptr;
    Camera* camera = nullptr;
    TileMap* map;

    std::unordered_map<std::string, std::string> getSettings() {
        return settings;
    }

    bool getSetting(const std::string& key, std::string& value) {

        const bool cont = settings.contains(key);
        value = cont ? settings[key] : value;

        return cont;
    }

    bool gameIsDebug() const {
        return isDebug;
    }

    static void setInstance(Knight_Game* kGame);

    Knight_Game() : Scene("Knight_Game_Main_Menu") {

        if(isDebug || !Parser::fileExist(SETTINGS_PATH)) {
            //setup and write settings here:

            //setup settings
            //settings["some string key"] = "some string value";
            //write settings
            Parser::writeFile(SETTINGS_PATH, settings);

        }
        else {
            settings = Parser::readFile(SETTINGS_PATH);
        }

        setInstance(this);
    }

    void onSceneLoad(SceneInformation sceneInfo) override;

    void createMap();

};


#endif //KNIGHT_GAME_H
