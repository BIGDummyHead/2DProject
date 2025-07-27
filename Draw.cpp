//
// Created by shawn on 5/16/2025.
//

#include "Draw.h"
#include <iostream>
#include <SDL_log.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <bits/stl_algo.h>
#include <SDL2/SDL_image.h>

#include "defs.h"
#include "Game/Camera.h"
#include "Game/Physics/Ray.h"
#include "Game/Physics/Raycaster.h"
#include "SDL_ttf/include/SDL_ttf.h"

Draw* Draw::toolInstance;
SDL_Renderer* Draw::renderer;

Draw *Draw::getInstance() {
    return toolInstance;
}

SDL_Renderer *Draw::getRenderer() {
    return renderer;
}



void Draw::prepareScene() const {
    SDL_SetRenderDrawColor(renderer, 96, 128, 255, 255);
    SDL_RenderClear(renderer);
}

void Draw::presentScene() const {
    SDL_RenderPresent(renderer);
}

SDL_Texture *Draw::loadTexture(const std::string &filePath) {
    if (loadedTextures.contains(filePath)) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "%s was already loaded.", filePath.c_str());
        return loadedTextures.at(filePath);
    }

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filePath.c_str());

    SDL_Texture *texture = IMG_LoadTexture(renderer, filePath.c_str());
    if (!texture) {
        throw std::runtime_error("Failed to load texture: " + filePath);
    }

    loadedTextures[filePath] = texture;

    return texture;
}

//Draws a texture at a given position. Set onlyCopy as a position from the texture to copy.
SDL_Rect Draw::blit(SDL_Texture *texture, const Vector2 position, const SDL_Rect *copySrc) const {
    SDL_Rect dest = position.asRect();

    SDL_QueryTexture(texture, nullptr, nullptr, &dest.w, &dest.h);

    SDL_RenderCopy(renderer, texture, copySrc, &dest);

    return dest;
}

SDL_Rect Draw::blitSheet(SDL_Texture *texture, const int rows, const int columns, const int renderRow,
                         const int renderCol, const Vector2 renderPosition, const Vector2 scalingFactor) const {
    int textWidth = 0;
    int textHeight = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);

    //based on the given texture, determine how big an object is
    const int spriteWidth = textWidth / columns;
    const int spriteHeight = textHeight / rows;

    //calculate the src rectangle
    const int srcX = spriteWidth * renderCol;
    const int srcY = spriteHeight * renderRow;
    const SDL_Rect srcRect = {srcX, srcY, spriteWidth, spriteHeight};

    //place where we should take from the texture
    SDL_Rect renderRectPosition = renderPosition.asRect();
    renderRectPosition.w = static_cast<int>(spriteWidth * scalingFactor.x);
    renderRectPosition.h = static_cast<int>(spriteHeight * scalingFactor.y);

    //center the object's render position
    renderRectPosition.x -= renderRectPosition.w / 2;
    renderRectPosition.y -= renderRectPosition.h / 2;

    // Draw a red rectangle behind the sprite
    //SDL_SetRenderDrawColor(getApp().renderer, 255, 0, 0, 255); // Red color
    //SDL_RenderFillRect(getApp().renderer, &dest);


    // Render the sprite
    SDL_RenderCopy(renderer, texture, &srcRect, &renderRectPosition);

    return renderRectPosition;
}

void Draw::drawLine(const Vector2 &from, const Vector2 &to) const {
    SDL_RenderDrawLineF(renderer, from.x, from.y, to.x, to.y);
}

void Draw::drawGradientLine(const Vector2 start, const Vector2 end, const double totalDistance, const Uint8 r,
                            const Uint8 g, const Uint8 b, const double intensity) const {

    const Vector2 delta = end - start;
    const double lineLength = delta.magnitude();

    const Vector2 direction = delta.normalized();

    // If the line is shorter than totalDistance, draw solid color line
    if (lineLength <= totalDistance) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, Uint8(r * intensity), Uint8(g * intensity), Uint8(b * intensity), 255);
        SDL_RenderDrawLine(renderer, (int) start.x, (int) start.y, (int) end.x, (int) end.y);
        return;
    }

    // Otherwise, draw the line pixel by pixel fading alpha from full to zero over totalDistance
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    const int steps = static_cast<int>(lineLength);
    for (int i = 0; i <= steps; ++i) {
        const double t = i / static_cast<float>(steps);
        const Vector2 point = start + direction * (lineLength * t);

        // Calculate distance from start along the line
        const double dist = lineLength * t;

        Uint8 alpha;
        if (dist <= totalDistance) {
            // Fade alpha linearly from 255 at start to 0 at totalDistance
            alpha = static_cast<Uint8>(255 * intensity * (1.0f - (dist / totalDistance)));
        } else {
            alpha = 0;
        }

        SDL_SetRenderDrawColor(renderer, r, g, b, alpha);
        SDL_RenderDrawPoint(renderer, static_cast<int>(point.x), static_cast<int>(point.y));
    }
}

static SDL_Texture *lightmap;
std::vector<LightSource> Draw::lightSources;

void Draw::addLightSource(const LightSource &source) {
    lightSources.push_back(source);
}


SDL_Texture *Draw::startLightMap() const {

    if (!lightmap) {
        //create a simple lightmap texture
        lightmap = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH,
                                     SCREEN_HEIGHT);
    }

    // Set lightmap as render target
    SDL_SetRenderTarget(renderer, lightmap);

    // Fill the whole texture with black (no light)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

    return lightmap;
}

void Draw::endLightMap(SDL_Texture *newDrawTexture) const {
    //add some base light into this.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 25);
    constexpr SDL_Rect screenRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderFillRect(renderer, &screenRect);

    SDL_SetRenderTarget(renderer, newDrawTexture); // Restore default
}

void Draw::drawLight(LightSource &light) const {

    //check if dynamic or not initalized
    if (light.isDynamic() || !light.hasDrawn) {
        // more rays = smoother cone
        const double centerAngle = light.angle * M_PI / 180.0;

        // Calculate start and end angles to cover the cone centered on centerAngle
        const double startAngle = centerAngle - light.radius / 2.0;
        const double endAngle = centerAngle + light.radius / 2.0;

        light.vertices.clear();
        const SDL_Color centerColor = {light.r, light.g, light.b, light.a}; // slightly dimmer if needed
        SDL_Vertex centerVertex{
            {static_cast<float>(light.position.x), static_cast<float>(light.position.y)},
            centerColor,
            {0, 0}
        };
        light.vertices.push_back(centerVertex);

        for (int i = 0; i < light.rayCastCount; ++i) {
            const double t = i / static_cast<double>(light.rayCastCount - 1); // from 0 to 1
            double rayAngle = startAngle + t * (endAngle - startAngle);

            Ray lightRay(light.position, rayAngle, light.distance);

            RayInfo rInfo;

            Vector2 end = {0, 0};

            if (light.createRayCastedShadowing && Raycaster::cast(lightRay, &rInfo)) {
                Vector2 toHit = rInfo.positionHit - light.position;
                const double hitDist = toHit.length();

                //TODO: check if this gObject that we hit has some kind of component like Shadow caster?
                //If it does then we can create a shadow for the object, that would be kinda epic!

                // Only overshoot if hit was significantly before full range (e.g. < 98%)
                if (hitDist < light.distance * 0.99) {
                    constexpr double overshootFactor = 1.15;
                    Vector2 direction = toHit.normalized();
                    double overshootDist = std::min(hitDist * overshootFactor, light.distance); // add 10 pixels max
                    end = light.position + direction * overshootDist;
                } else {
                    // Hit very close to end — no overshoot
                    end = rInfo.positionHit;
                }
            } else {
                end = Raycaster::createEndPoint(lightRay);
            }

            //copy and change up
            SDL_Color edgeColor = centerColor;
            edgeColor.a = 0;

            light.vertices.push_back({{static_cast<float>(end.x), static_cast<float>(end.y)}, edgeColor, {0, 0}});
        }

        light.indices.clear();
        // create indices for triangles
        for (int i = 1; i < light.vertices.size() - 1; ++i) {
            light.indices.push_back(0);
            light.indices.push_back(i);
            light.indices.push_back(i + 1);
        }

        light.hasDrawn = true;
    }

    SDL_RenderGeometry(renderer, nullptr, light.vertices.data(), light.vertices.size(), light.indices.data(),
                       light.indices.size());
}

void Draw::drawLights() const {
    for (auto light: lightSources) {
        drawLight(light);
    }
}

SDL_Texture* Draw::createTextTexture(const TextFont& uiFont, const char* text) const {
    //TTF_Font* font = TTF_OpenFont(R"(assets\fonts\font.ttf)", 24);
    TTF_Font *font = TTF_OpenFont(uiFont.getPath(), uiFont.size);
    if (!font) {
        SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Failed to load font: %s\n", TTF_GetError());
        return nullptr;
    }

    //constexpr SDL_Color white{255, 255, 255, 255};
    SDL_Surface *surface = nullptr;

    //TODO: Add in other render styles
    switch (uiFont.renderStyle) {
        case TextFont::Type::Solid:
            surface = TTF_RenderText_Solid(font, text, uiFont.color);
            break;
        default:
            SDL_LogError(SDL_LOG_PRIORITY_ERROR, "This render style is currently not supported\n");
            break;
    }

    if (!surface) {
        SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Failed to render solid text: %s\n", TTF_GetError());
        return nullptr;
    }


    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (!texture) {
        SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Failed to create texture from surface: %s\n", SDL_GetError());
        return nullptr;
    }

    SDL_FreeSurface(surface);
    TTF_CloseFont(font);

    //SDL_RenderCopy(renderer, texture, nullptr, destination);
    return texture;
}





