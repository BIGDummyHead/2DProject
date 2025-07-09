//
// Created by shawn on 6/25/2025.
//

#ifndef TEXTFONT_H
#define TEXTFONT_H
#include <SDL_pixels.h>
#include <SDL_stdinc.h>
#include <string>
#include <unordered_map>


//Options for rendering a font
class TextFont {
private:
    std::string fontName;
    const char *path;
    static constexpr SDL_Color DEFAULT_COLOR_WHITE{255, 255, 255, 255};
    static constexpr SDL_Color DEFAULT_BG_COLOR_TRANSPARENT{0, 0, 0, 0};
    static std::unordered_map<std::string, TextFont*> loadedFonts;

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

    [[nodiscard]] const char *getPath() const {
        return path;
    }

    TextFont(const std::string &fontName, const char *fontPath, const SDL_Color &foregroundColor,
             const bool saveFontByName = false, const int &fontSize = 16, const Type renderStyle = Solid,
             const SDL_Color &backgroundColor = SDL_Color{0, 0, 0, 0}, const Uint32 &wrap = 0) {

        this->fontName = fontName;
        path = fontPath;
        size = fontSize;
        this->renderStyle = renderStyle;
        this->wrap = wrap;
        color = foregroundColor;
        bgColor = backgroundColor;

        if (saveFontByName) {
            storeFont(this);
        }
    }

    TextFont(const std::string &fontName, const char *fontPath, const bool saveFontByName = false) : TextFont(
        fontName, fontPath, DEFAULT_COLOR_WHITE, saveFontByName) {
    }


    //Get the name of the font
    std::string getName();

    //Creates an exact replica of a font, does not store the font by name automatically.
    static const TextFont *copyFont(const TextFont *toCopy, bool saveFontByName = false);

    //Store this font for later user to retrieve by name.
    static void storeFont(TextFont *tryingToLoad);

    //Get a font by name
    static TextFont *getFontByName(const std::string &name);

    //Retrieve a stored font's path via the name. This can be used to recreate fonts with different settings.
    static const char *getFontPathByName(const std::string &name);

};


#endif //UIFONT_H
