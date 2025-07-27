//
// Created by shawn on 7/26/2025.
//

#ifndef APP_H
#define APP_H

#include <string>
#include <SDL.h>
#include <vector>
#include <SDL2/SDL_image.h>

#include "Vector2.h"


class App {
public:
    typedef Vector2 SCREEN_DIMENSIONS;
    typedef Uint32 WINDOW_FLAGS;
    typedef Uint32 RENDER_FLAGS;
    typedef int DRIVER_INDEX;
    typedef int SDL_IMAGE_FLAGS;

    struct Settings final {
    public:
        struct Hint final {
            std::string name;
            std::string value;

            bool failed = false;

            Hint(const std::string &name, const std::string &value) {
                this->name = name;
                this->value = value;
            }
        };

        Vector2 windowPosition;
        SCREEN_DIMENSIONS windowDimensions;
        WINDOW_FLAGS windowFlags;

        //The Renderer type to use, set to SDL_RENDERER_ACCELERATED initially
        mutable RENDER_FLAGS renderFlags = SDL_RENDERER_ACCELERATED;

        //Index of the driver to use, set to use to the first applicable
        mutable DRIVER_INDEX index = -1;

        //Hints to set for SDL, initially contains a render scale quality of linear.
        std::vector<Hint> sdlHints;

        //The types of images allowed to be used with SDL_IMG
        mutable SDL_IMAGE_FLAGS imageFlags = IMG_INIT_PNG | IMG_INIT_JPG;


        explicit Settings(const SCREEN_DIMENSIONS windowDimensions, const WINDOW_FLAGS flags) : Settings(
            windowDimensions,
            Vector2{SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED},
            flags) {
        }

        Settings(const SCREEN_DIMENSIONS windowDimensions, const Vector2 screenPosition, const WINDOW_FLAGS flags) {
            this->windowDimensions = windowDimensions;
            this->windowPosition = screenPosition;
            this->windowFlags = flags;
            sdlHints.push_back(Hint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"));
        }
    };

private:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    static App *applicationInstance;
    Settings* settings = nullptr;


    std::string name;

public:
    void setRenderer(SDL_Renderer *renderer);

    void setWindow(SDL_Window *window);

    [[nodiscard]] SDL_Window *getWindow() const;

    [[nodiscard]] SDL_Renderer *getRenderer() const;

    static App *getInstance();

    [[nodiscard]] Settings* getSettings() const;

    //Initializes SDL and tooling services. Returns nullptr on success and an error message on failure.
    const char *initialize(App::Settings *settings);

    explicit App(const std::string &name) {
        this->name = name;
        applicationInstance = this;
    }

    ~App() {
        delete settings;
        delete applicationInstance;
    }
};

#endif //APP_H
