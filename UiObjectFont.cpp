//
// Created by shawn on 6/30/2025.
//

#include "UiObjectFont.h"

void UiObjectFont::setRenderingText(const char *text) {
    if(text == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to set Rendering text because the ptr was null");
        return;
    }
    this->renderingText = text;
}

void UiObjectFont::render() {

    /*
    * SDL_Color foreGround(255, 255, 255, 255);
        UiFont font(R"(assets\fonts\font.ttf)", foreGround);
        SDL_Texture* text = drawTool.createTextTexture(font, "Hello Peyton");

        SDL_Rect textDest(100, 100, 500, 300);
        SDL_RenderCopy(drawTool.getApp().renderer, text, nullptr, &textDest);
     */

    auto* drawTool = Draw::getInstance();

    auto* text = drawTool->createTextTexture(*renderingFont, renderingText);

    drawTool->blit(text, position, nullptr);
}



