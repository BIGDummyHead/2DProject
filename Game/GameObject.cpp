//
// Created by shawn on 5/16/2025.
//

#include "GameObject.h"

#include <SDL_log.h>

std::unordered_map< int, std::vector<GameObject*>> GameObject::layeredGameObjects;
std::vector< int> GameObject::registeredLayers;

bool GameObject::getIsActive() const {
    return isActive;
}

void GameObject::update() {
}
void GameObject::start() {

}

Collider *GameObject::automaticCollider(const bool& setStatic, const bool& useSheet) const {
    if(!texture) {
        SDL_LogError(
            SDL_LOG_CATEGORY_ERROR,
            "Texture you are trying to generate a collider for was null");
        return nullptr;
    }
    //render this once for the following calls
    texture->render(transform->getPosition());

    const Vector2 box = useSheet ?
        Collider::createBoxFromSheet(*dynamic_cast<Sheet*>(texture)) :
        Collider::createBoxFromTexture(*texture);

    return new Collider(box.x, box.y, setStatic);
}




void GameObject::onRender(const Vector2& drawnAt) {

}

void GameObject::onCollision(Collider *other) {

}

void GameObject::removeFromBucket() {
    std::vector<GameObject*>* bucket = &layeredGameObjects[renderLayer];

    auto obj = std::ranges::find(*bucket, this);

    if(obj != bucket->end()) {
        bucket->erase(obj);
    }

    if(bucket->empty()) {
        //remove from buckets
        auto layerIter = layeredGameObjects.find(renderLayer);
        if(layerIter != layeredGameObjects.end()) {
            layeredGameObjects.erase(layerIter);
        }
    }
}

 int GameObject::getRenderLayer() const {
    return renderLayer;
}

void GameObject::setRenderLayer(const int &layer) {

    if(layeredGameObjects.contains(renderLayer)) {
        removeFromBucket();
    }

    renderLayer = layer;

    if(!layeredGameObjects.contains(renderLayer)) {
        layeredGameObjects[renderLayer] = {};
    }

    layeredGameObjects[renderLayer].push_back(this);

    const auto it =
        std::ranges::find(registeredLayers, renderLayer);

    if(it == registeredLayers.end()) {
        registeredLayers.push_back(renderLayer);
        std::ranges::sort(registeredLayers);
    }
}


Generator<GameObject*> GameObject::getGameObjects(bool includeInactive) {
    for(const auto&[layer, objs] : layeredGameObjects) {
        for(GameObject* obj : objs) {
            if(!includeInactive && !obj->isActive) {
                continue;
            }

            co_yield obj;
        }
    }

    co_return;
}







void GameObject::destroy() {
    setIsActive(false);
    delete collider;
    delete transform;

    //remove this from the bucket completely
    removeFromBucket();

    //inactiveObjects.erase(this);
    //registeredObjects.erase(this);
}

void GameObject::updateFrame() {

    if(!hasStarted) {
        start();
        hasStarted = true;
    }

    for(const auto comp : activeComponents) {

        if(!comp->hasStarted) {
            comp->start();
            comp->hasStarted = true;
            continue;
        }

        comp->update();
    }

    //do internal things
    update();
}







void GameObject::setIsActive(const bool status)  {
    isActive = status;
}







