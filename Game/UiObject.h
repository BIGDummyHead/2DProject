//
// Created by shawn on 6/14/2025.
//

#ifndef UIOBJECT_H
#define UIOBJECT_H
#include <SDL.h>

#include "Collider.h"
#include "../draw.h"


class UiObject {

private:
    static std::vector<UiObject*> registeredUI;
    bool mouseFocus = false;


public:
    virtual ~UiObject() = default;

    draw* drawTool = nullptr;
    Texture* drawingTexture;

    Vector2 position;

    //The UI's collider, that is interacted with via the mouse
    Collider* collider = nullptr;

    UiObject(draw* tool, Texture* texture) {
        drawTool = tool;
        drawingTexture = texture;

        registeredUI.push_back(this);

    }

    //Render this UI Object.
    virtual void render();

    void setMouseOut();
    void setMouseIn();

    //True when the mouse is over this object.
    [[nodiscard]] bool isMouseFocused() const;

    virtual void onMouseClick();
    virtual void onMouseRelease();
    virtual void whileMouseDown();

    virtual void whileMouseOver();
    virtual void whileMouseOut();
    virtual void onMouseExit();
    virtual void onMouseEnter();

    static std::vector<UiObject*> getRegisteredUI();




};



#endif //UIOBJECT_H
