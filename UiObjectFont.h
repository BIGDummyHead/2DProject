//
// Created by shawn on 6/30/2025.
//

#ifndef UIOBJECTFONT_H
#define UIOBJECTFONT_H
#include "Draw.h"
#include "TextFont.h"
#include "Game/UiObject.h"


class UiObjectFont final : public UiObject {

private:
    const char* renderingText;

public:
    TextFont *renderingFont;


    explicit UiObjectFont(TextFont* font, const char* text = nullptr) : UiObject(nullptr) {
        renderingFont = font;

        renderingText = text == nullptr ? "" : text;
        //texture can be left null for right now
    }

    void setRenderingText(const char* text);
    void render() override;


};



#endif //UIFONT_H
