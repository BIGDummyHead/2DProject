//
// Created by shawn on 5/16/2025.
//

#ifndef DRAW_H
#define DRAW_H
#include <map>
#include <SDL_render.h>
#include <vector>

#include "Vector2.h"
#include "Game/LightSource.h"
#include "TextFont.h"
#include "App.h"

//Class to help with drawing features and scene rendering
class Draw {

private:
    std::map<std::string, SDL_Texture*> loadedTextures;
    static Draw* toolInstance;
    static SDL_Renderer* renderer;

public:

    static SDL_Renderer* getRenderer();
    static Draw* getInstance();


    explicit Draw() {
        toolInstance = this;
        renderer = App::getInstance()->getRenderer();
        if(!renderer) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not allocate a renderer for drawing tool");
            exit(-1);
        }
    }

    void prepareScene() const;
    void presentScene() const;
    [[nodiscard]] SDL_Texture* loadTexture(const std::string& filePath);

    SDL_Rect blit(SDL_Texture *texture, Vector2 position, const SDL_Rect* copySrc = nullptr) const;

    SDL_Rect blitSheet(SDL_Texture *texture, int rows, int columns, int renderRow, int renderCol, Vector2 renderPosition, Vector2 scalingFactor) const;

    void drawLine(const Vector2& from, const Vector2& to) const;

    static std::vector<LightSource*> lightSources;

    //Add a light source to be rendered
    void addLightSource(LightSource* source);
    //Start the light map texture and return it
    [[nodiscard]] SDL_Texture* startLightMap() const;
    //Draw a singular light, can be useful for certain situations
    void drawLight(LightSource* light) const;
    //Draws all light sources that have been registered.
    void drawLights() const;
    //Completes and renders the light map, returning to the newDrawTexture or default.
    void endLightMap(SDL_Texture* newDrawTexture = nullptr) const;

    void drawGradientLine(Vector2 start, Vector2 end, double totalDistance, Uint8 r, Uint8 g, Uint8 b, double intensity) const;

    //[[nodiscard]] SDL_Texture* createTextTexture(UiFont& uiFont, const char* text) const;
    SDL_Texture* createTextTexture(const TextFont& uiFont, const char* text) const;
};



#endif //DRAW_H
