//
// Created by shawn on 6/25/2025.
//

#ifndef UIFONT_H
#define UIFONT_H
#include <SDL_pixels.h>
#include <SDL_stdinc.h>

#endif //UIFONT_H

//Options for rendering a font
class UiFont {

private:
    const char* path;

public:

    enum Type {
        Solid,
        Blended,
        Shaded,
        LCD,
        Blended_Wrapped,
        Shaded_Wrapped,
        Solid_Wrapped,
        LCD_Wrapped
    };

    int size;
    Type renderStyle;
    //Used for wrapped render styles
    Uint32 wrap;

    SDL_Color color{};
    SDL_Color bgColor{};

    [[nodiscard]] const char* getPath() const {
        return path;
    }

    UiFont(const char* fontPath, const SDL_Color& foregroundColor, const int& fontSize = 16, const Type renderStyle = Solid, const SDL_Color& backgroundColor = {255,255,255,255}, const Uint32& wrap = 0) {
        path = fontPath;
        size = fontSize;
        this->renderStyle = renderStyle;
        this->wrap = wrap;
        color = foregroundColor;
        bgColor = backgroundColor;
    }
};