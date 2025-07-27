//
// Created by shawn on 7/26/2025.
//

#ifndef APP_H
#define APP_H

#include <SDL.h>

class App {

private:
    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = nullptr;
    static App* applicationInstance;

public:
    const char* name = nullptr;

    void setRenderer(SDL_Renderer* renderer);
    void setWindow(SDL_Window* window);

    [[nodiscard]] SDL_Window* getWindow() const;
    [[nodiscard]] SDL_Renderer* getRenderer() const;

    static App* getInstance();

    App() {
        applicationInstance = this;
    }

};

#endif //APP_H
