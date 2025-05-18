#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "draw.h"
#include "init.h"
#include "input.h"
#include "Game/GObject.h"
#include "Game/Sheet.h"
#include <chrono>

SDL_Texture* createPlayer(draw& tool, Vector2& position) {

    SDL_Texture* playerText = tool.loadTexture(R"(assets\player\Idle\Character_Idle.png)");

    position.x = 100;
    position.y = 100;

    return  playerText;
}

class Test_Player : public GObject {

private:
    Sheet textureSheet;

public:

    explicit Test_Player(draw* drawTool) : GObject(drawTool, "The Player"), textureSheet(drawTool->loadTexture(R"(assets\player\Idle\Character_Idle.png)"), 4, 6) {
        textureSheet.scale *= 2.5;
        texture = &textureSheet;
        transform->position = Vector2(100, 100);
    }

    float getHorizontalMove() {

        float h = 0;

        if(input::isKeyDown(SDLK_a) ||input::isKeyHeld(SDLK_a)) {
            h--;
        }

        if(input::isKeyDown(SDLK_d) ||input::isKeyHeld(SDLK_d)) {
            h++;
        }

        return h;
    }

    float getVerticalMove() {
        float v = 0;

        if(input::isKeyDown(SDLK_w) || input::isKeyHeld(SDLK_w)) {
            v--;
        }

        if(input::isKeyDown(SDLK_s) ||input::isKeyHeld(SDLK_s)) {
            v++;
        }

        return v;
    }

    void update() override {
        Vector2 move(getHorizontalMove(), getVerticalMove());
        move *= 4;
        transform->position += move;
    }
};

[[noreturn]] int main() {

    App myApp;
    myApp.name = "My Game!";

    init::initSDL(myApp);

    draw drawTool(&myApp);

    Test_Player playerObject(&drawTool);

    auto elapsed_time = std::chrono::steady_clock::time_point();
    auto frame_time = std::chrono::steady_clock::time_point();

    while(true) {
        //game loop
        drawTool.prepareScene();

        input::pollInput();

        for(auto activeObj : GObject::activeObjects) {

            if(activeObj == nullptr || activeObj->transform == nullptr) {
                continue;
            }

            activeObj->texture->render(drawTool, activeObj->transform->position);

            auto now = std::chrono::steady_clock::time_point();
            const long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frame_time - now).count();

            if(elapsed > 10) {
                frame_time = now; //reset clock to 0
                activeObj->update();
            }
        }


        drawTool.presentScene();

        constexpr Uint32 sdlDelayMS = 5;
        SDL_Delay(sdlDelayMS);
        elapsed_time += std::chrono::milliseconds(sdlDelayMS);
        frame_time += std::chrono::milliseconds(sdlDelayMS);
    }

}
