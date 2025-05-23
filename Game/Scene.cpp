//
// Created by shawn on 5/23/2025.
//

#include "Scene.h"

Scene* Scene::currentlyLoadedScene = nullptr;
std::unordered_map<std::string, Scene*> Scene::createdScenes;

void Scene::onSceneLoad(SceneInformation sceneInfo) {

}

std::string Scene::getName() {
    return this->name;
}

void Scene::loadScene(const std::string &name, const SceneInformation scene)  {

    //destroys the current loaded scene if any
    destroyScene();

    if(!createdScenes.contains(name)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "No scene with this name exist");
        return;
    }

    Scene* loadingScene = createdScenes[name];
    loadingScene->onSceneLoad(scene);
    currentlyLoadedScene = loadingScene;
}

void Scene::destroyScene() {
    if (currentlyLoadedScene == nullptr) //destroy the scene
        return;


    std::vector<GObject*> cleared;

    for (const auto obj: GObject::registeredObjects) {
        //* removed line from destory that would remove registeredObjects key, causing modification of list
        if (obj->destroyOnSceneLoad) {
            //destroy the item and add the item to the clearing item
            obj->destroy();
            cleared.push_back(obj);
        }
    }

    //delete all that were not in the cleared subcat
    for(const auto clearing : cleared) {
        GObject::registeredObjects.erase(clearing);
    }

    currentlyLoadedScene = nullptr;
}



