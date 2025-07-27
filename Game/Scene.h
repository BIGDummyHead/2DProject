//
// Created by shawn on 5/23/2025.
//

#ifndef SCENE_H
#define SCENE_H

#include "GObject.h"
#include <unordered_map>

typedef struct {
    App* runningApplication;
    Draw* drawingTool;
} SceneInformation;

class Scene {

private:
    static Scene* currentlyLoadedScene;
    std::string name;

    static std::unordered_map<std::string, Scene*> createdScenes;

public:
    virtual ~Scene() = default;


    std::string getName();


    explicit Scene(const std::string& name) {

        if(createdScenes.contains(name))
            throw std::runtime_error("You cannot create a scene with a duplicate name!");

        this->name = name;
        createdScenes[name] = this;
    }

    virtual void onSceneLoad(SceneInformation sceneInfo);

    static void loadScene(const std::string& name, SceneInformation scene);
    static void loadScene(Scene* scene, SceneInformation sceneInfo);

    //Destroy the currentlyLoadedScene if there is any scene.
    static void destroyScene();

    static void loadFirstAvalScene(SceneInformation scene);
};



#endif //SCENE_H
