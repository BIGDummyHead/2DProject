//
// Created by shawn on 5/16/2025.
//

#ifndef GOBJECT_H
#define GOBJECT_H

#include <stdio.h>
#include <string>
#include <vector>

#include <unordered_set>

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


    void setIsActive(const bool status) {
        if(isActive && status || !isActive && !status) { //there is nothing tto do
            return;
        }

        isActive = status;

        if(isActive) {

            const auto found = inactiveObjects.find(this);

            if(found != inactiveObjects.end()) {
                inactiveObjects.erase(*found);
            }

            activeObjects.insert(*found);

        }
        else {
            const auto found = activeObjects.find(this);

            if(found != activeObjects.end()) {
                activeObjects.erase(*found);
            }

            inactiveObjects.insert(*found);
        }
    }


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
    virtual void onRender();

};



#endif //GOBJECT_H
