//
// Created by shawn on 7/1/2025.
//

#include "TextFont.h"

std::unordered_map<std::string, TextFont*> TextFont::loadedFonts;

//Get the name of the font
std::string TextFont::getName() {
    return this->fontName;
}

//Creates an exact replica of a font, does not store the font by name automatically.
const TextFont * TextFont::copyFont(const TextFont *toCopy, const bool saveFontByName) {
    const TextFont *copied = new TextFont(toCopy->fontName, toCopy->path, toCopy->color, saveFontByName,
                                          toCopy->size,
                                          toCopy->renderStyle, toCopy->bgColor, toCopy->wrap);

    return copied;
}

//Store this font for later user to retrieve by name...
void TextFont::storeFont(TextFont *tryingToLoad) {
    if (tryingToLoad == nullptr || loadedFonts.contains(tryingToLoad->fontName)) {
        return;
    }

    loadedFonts[tryingToLoad->fontName] = tryingToLoad;
}

//Get a font by name
TextFont* TextFont::getFontByName(const std::string &name) {
    return loadedFonts.contains(name) ? loadedFonts[name] : nullptr;
}

//Retrieve a stored font's path via the name. This can be used to recreate fonts with different settings.
const char * TextFont::getFontPathByName(const std::string &name) {
    const TextFont *font = getFontByName(name);
    return font == nullptr ? nullptr : font->path;
}