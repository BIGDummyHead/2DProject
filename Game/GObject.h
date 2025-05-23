//
// Created by shawn on 5/16/2025.
//

#ifndef GOBJECT_H
#define GOBJECT_H
#include <string>
#include <unordered_set>
#include "Collider.h"
#include "Texture.h"
#include "Transform.h"

class GObject {

private:
    bool isActive = true;

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
    }

    explicit GObject(draw* drawTool, const std::string &name) : GObject(drawTool) {
        this->name = name;
    }

    virtual void update();
    virtual  void onRender(const Vector2& drawnAt);
    virtual  void onCollision(Collider* other);

    void destroy();


};



#endif //GOBJECT_H
