#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "draw.h"
#include "init.h"
#include "input.h"
#include "Game/GObject.h"
#include "Game/Sheet.h"
#include <chrono>

#include "Game/Camera.h"
#include "Game/Collider.h"


class Test_Player final : public GObject {

private:
    Sheet textureSheet;

public:

    Collider* collider;
    Camera* cam = nullptr;
    explicit Test_Player(draw* drawTool, const Vector2& pos) : GObject(drawTool, "The Player"), textureSheet(drawTool->loadTexture(R"(assets\player\Idle\Character_Idle.png)"), 4, 4) {
        textureSheet.scale *= 2.5;
        texture = &textureSheet;
        transform->setPosition(pos);

        collider = new Collider(transform, -24, 40, 24, -40);
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
        move = cam->transform->getPosition() - move;
        //transform->setPosition(move);
        cam->transform->setPosition(move);
        //transform->setPosition(move);
    }

    void onRender() override {
        collider->drawDebugCollider(drawTool);
    }

};

[[noreturn]] int main() {

    //Initialize the application
    App myApp;
    myApp.name = "My Game!";
    init::initSDL(myApp);

    //create a drawtool from application
    draw drawTool(&myApp);

    //Center of the Screen, can be used for rendering
    const Vector2 center(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    //Create a camera to use for later, this will control scrolling
    Camera cam(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT), center);

    //Create a new basic gameobjects, using sheets

    /*Death Object*/
    GObject obj(&drawTool, "This is a game object");
    Sheet sheet(drawTool.loadTexture(R"(assets\player\Death\Character_Death.png)"), 4, 11);
    sheet.scale *= 2.5;
    obj.texture = &sheet;


    /* Player Object */
    Test_Player playerObject(&drawTool, center);
    playerObject.cam = &cam; //hook the camera up to the GObject


    //Setup timers for appropriate updating functionality.
    auto elapsed_time = std::chrono::steady_clock::time_point();
    auto frame_time = std::chrono::steady_clock::time_point();
    while(true) {
        //Prepare the scene for rendering
        drawTool.prepareScene();

        //Poll for inputs, updates the input sectors
        input::pollInput();

        //create a time for now to determine timing
        auto now = std::chrono::steady_clock::time_point();
        const long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frame_time - now).count();

        //Loop over active scene objects.
        for(auto activeObj : GObject::activeObjects) {

            //Ensure this object is not null for drawing
            if(activeObj == nullptr || activeObj->transform == nullptr) {
                continue;
            }

            //hard coded, TODO: change how this part works
            if(activeObj == &playerObject) {
                //draw normally, this way the player object is followed
                activeObj->texture->render(drawTool, activeObj->transform->getPosition());
            }
            else if(cam.isInRenderView(activeObj->transform->getPosition())){
                //Create an offset of where the item will be drawn
                const Vector2 where = cam.transform->getPosition() - activeObj->transform->getPosition();
                activeObj->texture->render(drawTool, where);
            }

            activeObj->onRender();



            if(elapsed > UPDATE_DELAY_MS) { //Call the update
                frame_time = now; //reset clock to 0
                activeObj->update();
            }
        }


        //Draw the render
        drawTool.presentScene();

        //Give a delay between render
        SDL_Delay(RENDER_DELAY_MS);
        elapsed_time += std::chrono::milliseconds(RENDER_DELAY_MS);
        frame_time += std::chrono::milliseconds(RENDER_DELAY_MS);
    }

}
