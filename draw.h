//
// Created by shawn on 5/16/2025.
//

#ifndef DRAW_H
#define DRAW_H
#include "structs.h"
#include <map>
#include <SDL_render.h>
#include <vector>

#include "Vector2.h"
#include "Game/LightSource.h"
#include "UiFont.h"

//Class to help with drawing features and scene rendering
class draw {

private:
    App* drawingFor;
    std::map<std::string, SDL_Texture*> loadedTextures;

public:
    [[nodiscard]] App getApp() const;


    explicit draw(App* drawingFor) {
        this->drawingFor = drawingFor;
    }

    void prepareScene() const;
    void presentScene() const;
    [[nodiscard]] SDL_Texture* loadTexture(const std::string& filePath);

    SDL_Rect blit(SDL_Texture *texture, Vector2 position, const SDL_Rect* copySrc = nullptr) const;

    SDL_Rect blitSheet(SDL_Texture *texture, int rows, int columns, int renderRow, int renderCol, Vector2 renderPosition, Vector2 scalingFactor) const;

    void drawLine(const Vector2& from, const Vector2& to) const;

    static std::vector<LightSource> lightSources;

    //Add a light source to be rendered
    void addLightSource(const LightSource& source);
    //Start the light map texture and return it
    [[nodiscard]] SDL_Texture* startLightMap() const;
    //Draw a singular light, can be useful for certain situations
    void drawLight(LightSource& light) const;
    //Draws all light sources that have been registered.
    void drawLights() const;
    //Completes and renders the light map, returning to the newDrawTexture or default.
    void endLightMap(SDL_Texture* newDrawTexture = nullptr) const;

    void drawGradientLine(Vector2 start, Vector2 end, double totalDistance, Uint8 r, Uint8 g, Uint8 b, double intensity) const;

    [[nodiscard]] SDL_Texture* createTextTexture(UiFont uiFont&, const char* text) const;
};



#endif //DRAW_H
