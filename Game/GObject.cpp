//
// Created by shawn on 5/16/2025.
//

#include "GObject.h"

#include <SDL_log.h>

std::unordered_map< int, std::vector<GObject*>> GObject::layeredGameObjects;
std::vector< int> GObject::registeredLayers;

bool GObject::getIsActive() const {
    return isActive;
}

void GObject::update() {
}
void GObject::start() {

}

Collider *GObject::automaticCollider(const bool& setStatic, const bool& useSheet) const {
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




void GObject::onRender(const Vector2& drawnAt) {

}

void GObject::onCollision(Collider *other) {

}

void GObject::removeFromBucket() {
    std::vector<GObject*>* bucket = &layeredGameObjects[renderLayer];

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

 int GObject::getRenderLayer() const {
    return renderLayer;
}

void GObject::setRenderLayer(const int &layer) {

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


Generator<GObject*> GObject::getGameObjects(bool includeInactive) {
    for(const auto&[layer, objs] : layeredGameObjects) {
        for(GObject* obj : objs) {
            if(!includeInactive && !obj->isActive) {
                continue;
            }

            co_yield obj;
        }
    }

    co_return;
}







void GObject::destroy() {
    setIsActive(false);
    delete collider;
    delete transform;

    //remove this from the bucket completely
    removeFromBucket();

    //inactiveObjects.erase(this);
    //registeredObjects.erase(this);
}

void GObject::updateFrame() {

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







void GObject::setIsActive(const bool status)  {
    isActive = status;
}







