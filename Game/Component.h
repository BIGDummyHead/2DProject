//
// Created by shawn on 6/14/2025.
//

#ifndef COMPONENT_H
#define COMPONENT_H

#pragma once
class GObject;

class Component {

private:
    GObject* attachedObject = nullptr; //Set later.

public:
    bool hasStarted = false;

    virtual ~Component() = default;

    //Called when you add a component to a GObject.
    virtual void awake();
    //Called before the @update function, only once.
    virtual void start();
    //Called every frame after the @start function.
    virtual void update();
    //Called when the component is removed.
    virtual void destroy();

    //Sets the attached object pointer, should not be touched.
    void setAttachedObject(GObject* obj);
    //Gets the attached object this component was set to.
    [[nodiscard]] GObject* getAttachedObject() const;



};



#endif //COMPONENT_H
