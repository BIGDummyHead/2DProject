//
// Created by shawn on 5/23/2025.
//

#include "Scene.h"

#include <SDL_log.h>

Scene* Scene::currentlyLoadedScene = nullptr;
std::unordered_map<std::string, Scene*> Scene::createdScenes;

void Scene::onSceneLoad(SceneInformation sceneInfo) {

}

std::string Scene::getName() {
    return this->name;
}

void Scene::loadScene(const std::string &name, const SceneInformation scene)  {

    if(!createdScenes.contains(name)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "No scene with this name exist");
        return;
    }

    Scene* loadingScene = createdScenes[name];

    loadScene(loadingScene, scene);

}

void Scene::loadScene(Scene *scene, const SceneInformation sceneInfo) {

    if(scene == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Invalid scene tried loading");
        return;
    }
    //destroys the current loaded scene if any
    destroyScene();

    scene->onSceneLoad(sceneInfo);
    currentlyLoadedScene = scene;
}


void Scene::loadFirstAvalScene(const SceneInformation scene) {

    loadScene(createdScenes.begin()->first, scene);

}


void Scene::destroyScene() {
    if (currentlyLoadedScene == nullptr) //destroy the scene
        return;


    std::vector<GameObject*> cleared;

    auto coroutineGameObjects =
        GameObject::getGameObjects(true);

    for(int i = 0; coroutineGameObjects; i++) {
        auto obj = coroutineGameObjects();
        //* removed line from destory that would remove registeredObjects key, causing modification of list
        if (obj->destroyOnSceneLoad) {
            //destroy the item and add the item to the clearing item
            obj->destroy();
            cleared.push_back(obj);
        }
    }

    //delete all that were not in the cleared subcat
    //for(const auto clearing : cleared) {
      //  GObject::registeredObjects.erase(clearing);
    //}

    currentlyLoadedScene = nullptr;
}



