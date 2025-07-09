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

class GObject {

private:
    bool isActive = true;
    std::vector<Component*> activeComponents{};
    bool hasStarted = false;

public:
    virtual ~GObject() = default;

    static std::unordered_set<GObject*> registeredObjects;
    static  std::unordered_set<GObject*> activeObjects;
    static std::unordered_set<GObject*> inactiveObjects;



    std::string name;
    Transform* transform;
    Texture* texture = nullptr;
    draw* drawTool;
    Collider* collider = nullptr;

    bool destroyOnSceneLoad = true;


    void setIsActive(bool status);


    [[nodiscard]] bool getIsActive() const;

    explicit GObject(draw* drawTool) {

        this->drawTool = drawTool;
        if(name.empty()) {
            name = "GObject " + registeredObjects.size();
        }

        registeredObjects.insert(this);
        activeObjects.insert(this);

        transform = new Transform();
        GObject::start();
    }

    explicit GObject(draw* drawTool, const std::string &name) : GObject(drawTool) {
        this->name = name;
    }




    //Automatically create a collider based on your texture (if it is a sheet), does not set the collider
    Collider* automaticCollider(const bool& setStatic, const bool& useSheet = false) const;
    void updateFrame();

    template<typename T> requires std::is_base_of_v<Component, T>
    void addComponent() {
        T* componentPtr = new T();
        activeComponents.push_back(componentPtr);

        auto* comp = dynamic_cast<Component *>(componentPtr);
        comp->setAttachedObject(this);
        comp->awake();
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
