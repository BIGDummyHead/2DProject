//
// Created by shawn on 5/16/2025.
//

#ifndef GOBJECT_H
#define GOBJECT_H
#include <string>
#include <unordered_set>
#include "Collider.h"
#include "Component.h"
#include "Texture.h"
#include "Transform.h"
#include <algorithm>
#include <coroutine>

#include "../Generator.h"

class GameObject {

private:
    bool isActive = true;
    std::vector<Component*> activeComponents{};
    bool hasStarted = false;
    unsigned int renderLayer = 0;


    void removeFromBucket();

public:
    virtual ~GameObject() = default;

    static std::vector<int> registeredLayers;
    static std::unordered_map<int, std::vector<GameObject*>> layeredGameObjects;

    static Generator<GameObject*> getGameObjects(bool includeInactive = false);


    [[nodiscard]] int getRenderLayer() const;
    //Set the render layer.
    void setRenderLayer(const int& layer);

    std::string name;
    Transform* transform;
    Texture* texture = nullptr;
    Collider* collider = nullptr;

    bool destroyOnSceneLoad = true;


    void setIsActive(bool status);


    [[nodiscard]] bool getIsActive() const;

    explicit GameObject(const int layerIndex = 0) {

        if(name.empty()) {
            name = "GObject " + layeredGameObjects.size();
        }

        //setup the render layer
        setRenderLayer(layerIndex);

        transform = new Transform();
        GameObject::start();
    }

    explicit GameObject(const std::string &name, const int layerIndex = 0) : GameObject(layerIndex) {
        this->name = name;
    }

    //Automatically create a collider based on your texture (if it is a sheet), does not set the collider
    [[nodiscard]] Collider* automaticCollider(const bool& setStatic, const bool& useSheet = false) const;
    void updateFrame();

    template<typename T> requires std::is_base_of_v<Component, T>
    T* addComponent() {
        T* componentPtr = new T();
        activeComponents.push_back(componentPtr);

        auto* comp = dynamic_cast<Component *>(componentPtr);
        comp->setAttachedObject(this);
        comp->awake();
        return componentPtr;
    }

    template<typename T> requires std::is_base_of_v<Component, T>
    bool hasComponent() {
        return getComponent<T>() != nullptr;
    }


    template<typename T> requires std::is_base_of_v<Component, T>
    T* getComponent() {
        for(const auto comp : activeComponents) {
            T* currentPtr = dynamic_cast<T*>(comp);
            if(currentPtr != nullptr)
                return currentPtr;
        }

        return nullptr;
    }

    template<typename T> requires std::is_base_of_v<Component, T>
    bool removeComponent() {

        int removingIndex = -1;
        Component* removingComp = nullptr;
        for(int i = 0; i < activeComponents.size(); i++) {
            T* comp = dynamic_cast<T*>( activeComponents[i] );

            if(comp != nullptr) {
                removingIndex = i;
                removingComp = comp;
                break;
            }
        }

        if(removingIndex != -1) {

            activeComponents.erase(activeComponents.begin() + removingIndex);

            if(removingComp != nullptr) {
                removingComp->destroy();
                delete removingComp;
            }
        }

        return removingIndex != -1;
    }


    virtual void start();
    virtual void update();
    virtual void onRender(const Vector2& drawnAt);
    virtual void onCollision(Collider* other);

    void destroy();


};



#endif //GOBJECT_H
