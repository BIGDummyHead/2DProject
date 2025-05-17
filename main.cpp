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
public:
    void update() override {

    }
};

[[noreturn]] int main() {

    App myApp;
    myApp.name = "My Game!";

    init::initSDL(myApp);

    draw drawTool(&myApp);

    GObject playerObject("Custom Name");
    Sheet sheet(drawTool.loadTexture(R"(assets\player\Attack\Character_Attack.png)"), 4, 6);
    sheet.scale = Vector2(4, 4);
    playerObject.texture = &sheet;
    playerObject.transform->position = Vector2(100, 100);

    auto elapsed_time = std::chrono::steady_clock::time_point();
    auto frame_time = std::chrono::steady_clock::time_point();

    while(true) {
        //game loop
        drawTool.prepareScene();

        input::doInput();

        for(auto activeObj : GObject::activeObjects) {

            if(activeObj == nullptr || activeObj->transform == nullptr) {
                continue;
            }

            activeObj->texture->render(drawTool, activeObj->transform->position);

            auto now = std::chrono::steady_clock::time_point();
            const long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frame_time - now).count();

            if(elapsed > 100) {
                frame_time = now; //reset clock to 0
                activeObj->update();
            }
        }


        drawTool.presentScene();

        constexpr Uint32 sdlDelayMS = 16;
        SDL_Delay(sdlDelayMS);
        elapsed_time += std::chrono::milliseconds(sdlDelayMS);
        frame_time += std::chrono::milliseconds(sdlDelayMS);
    }

}
