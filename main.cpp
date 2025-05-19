#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "draw.h"
#include "init.h"
#include "input.h"
#include "Game/GObject.h"
#include "Game/Sheet.h"
#include <chrono>
#include "Game/Collider.h"


class Test_Player final : public GObject {

private:
    Sheet textureSheet;

public:

    Collider* collider;
    explicit Test_Player(draw* drawTool) : GObject(drawTool, "The Player"), textureSheet(drawTool->loadTexture(R"(assets\player\Idle\Character_Idle.png)"), 4, 6) {
        textureSheet.scale *= 2.5;
        texture = &textureSheet;
        transform->setPosition(Vector2(100, 100));

        collider = new Collider(transform, 135, 120, 180, 200);
    }

    static float getHorizontalMove() {

        float h = 0;

        if(input::isKeyHeld(SDLK_a)) {
            h--;
        }

        if(input::isKeyHeld(SDLK_d)) {
            h++;
        }

        return h;
    }

    static float getVerticalMove() {
        float v = 0;

        if(input::isKeyHeld(SDLK_w))
            v--;

        if(input::isKeyHeld(SDLK_s))
            v++;

        return v;
    }

    void update() override {
        Vector2 move(getHorizontalMove(), getVerticalMove());
        move *= 4;
        transform->setPosition(transform->getPosition() + move);
    }

    void onRender() override {
        collider->drawDebugCollider(drawTool);
    }

};

[[noreturn]] int main() {

    App myApp;
    myApp.name = "My Game!";

    init::initSDL(myApp);

    draw drawTool(&myApp);

    Test_Player playerObject(&drawTool);
    GObject obj(&drawTool, "This is a game object");

    Sheet sheet(drawTool.loadTexture(R"(assets\player\Death\Character_Death.png)"), 4, 11);
    sheet.scale *= 2.5;
    obj.texture = &sheet;

    obj.transform->setPosition(Vector2(250, 100));
    const auto* deathCollider = new Collider(obj.transform, 135, 120, 180, 200);



    auto elapsed_time = std::chrono::steady_clock::time_point();
    auto frame_time = std::chrono::steady_clock::time_point();

    while(true) {
        //game loop
        drawTool.prepareScene();

        input::pollInput();

        auto now = std::chrono::steady_clock::time_point();
        const long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frame_time - now).count();
        for(auto activeObj : GObject::activeObjects) {

            if(activeObj == nullptr || activeObj->transform == nullptr) {
                continue;
            }

            activeObj->texture->render(drawTool, activeObj->transform->getPosition());
            activeObj->onRender();
            deathCollider->drawDebugCollider(&drawTool);

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
