//
// Created by shawn on 8/1/2025.
//

#ifndef LIGHTSOURCECOMPONENT_H
#define LIGHTSOURCECOMPONENT_H
#include "Component.h"
#include "GameObject.h"



class LightSourceComponent final : public Component {
private:
    LightSource *source;

public:
    ~LightSourceComponent() {
        delete source;
    }

    void setSource(LightSource *source);

    //Called when you add a component to a GObject.
    void awake() override;

    //Called before the @update function, only once.
    void start() override;

    //Called every frame after the @start function.
    void update() override;

    //Called when the component is removed.
    void destroy() override;
};


#endif //LIGHTSOURCECOMPONENT_H
