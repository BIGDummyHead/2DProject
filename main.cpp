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
    Vector2 initSpawn;

    Sheet textureSheet = Sheet(drawTool->loadTexture(R"(assets\player\Idle\Character_Idle.png)"), 4, 4);
    Sheet runningSheet = Sheet(drawTool->loadTexture(R"(assets\player\Move\Character_Move.png)"), 4, 6);

    std::chrono::steady_clock::time_point lastAnimationTime = std::chrono::steady_clock::now();
    const int animationDelayMs = 95; // Delay in milliseconds

public:
    Camera *cam = nullptr;

    explicit Test_Player(draw *drawTool, const Vector2 &pos) : GObject(drawTool, "The Player"), initSpawn(pos) {
        textureSheet.scale *= 2.5;
        runningSheet.scale *= 2.5;

        texture = &textureSheet;
        transform->setPosition(pos);

        textureSheet.render(*drawTool, Vector2());
        const Vector2 box = Collider::createBoxFromTexture(textureSheet);
        collider = new Collider(box.x, box.y, false);
    }

    static float getHorizontalMove() {
        float h = 0;

        if (input::isKeyHeld(SDLK_a)) {
            h--;
        }

        if (input::isKeyHeld(SDLK_d)) {
            h++;
        }

        return h;
    }

    static float getVerticalMove() {
        float v = 0;

        if (input::isKeyHeld(SDLK_w))
            v--;

        if (input::isKeyHeld(SDLK_s))
            v++;

        return v;
    }

    bool shouldAnimate() {
        const auto now = std::chrono::steady_clock::now();
        const int elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastAnimationTime).count();

        if (elapsedMs < animationDelayMs)
            return false;

        lastAnimationTime = now;
        return true;
    }

    void animateMovement(const Vector2 &move) {
        if (!shouldAnimate()) {
            return;
        }

        texture = &runningSheet;
        if (move.x > 0) {
            runningSheet.setRow(2);
        } else if (move.x < 0) {
            runningSheet.setRow(1);
        }

        if (move.y > 0) {
            runningSheet.setRow(0);
        } else if (move.y < 0) {
            runningSheet.setRow(3);
        }
        runningSheet.moveColRight(true);
    }

    void doMovement(Vector2 &move) const {
        move *= 4;

        // Temporarily update the player's position
        transform->setPosition(transform->getPosition() + move);

        // After collision resolution, update the camera's position
        if (cam != nullptr) {
            cam->transform->setPosition(transform->getPosition() - (initSpawn));
        }
    }

    void update() override {
        Vector2 move(getHorizontalMove(), getVerticalMove());
        if (move.magnitude() != 0) {
            animateMovement(move);
            doMovement(move);
        } else {
            if (texture != &textureSheet) {
                texture = &textureSheet;
            }

            if (shouldAnimate()) {
                textureSheet.setRow(0);
                textureSheet.moveColRight(true);
            }
        }
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
    Camera cam(Vector2(SCREEN_WIDTH + 10, SCREEN_HEIGHT + 10), Vector2{0, 0});

    /*Death Object*/
    GObject obj(&drawTool, "This is a game object");
    Sheet sheet(drawTool.loadTexture(R"(assets\player\Death\Character_Death.png)"), 4, 11);
    obj.collider = new Collider(20, 40);
    obj.transform->setPosition(center + Vector2{-250, 0});
    sheet.scale *= 2.5;
    obj.texture = &sheet;


    /* Player Object */
    Test_Player playerObject(&drawTool, center);
    playerObject.cam = &cam; //hook the camera up to the GObject


    //Setup timers for appropriate updating functionality.
    auto elapsed_time = std::chrono::steady_clock::time_point();
    auto frame_time = std::chrono::steady_clock::time_point();
    while (true) {
        //Prepare the scene for rendering
        drawTool.prepareScene();

        //Poll for inputs, updates the input sectors
        input::pollInput();

        //create a time for now to determine timing
        auto now = std::chrono::steady_clock::time_point();
        const long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frame_time - now).count();


        //Loop over active scene objects.
        for (auto activeObj: GObject::activeObjects) {
            //Ensure this object is not null for drawing
            if (activeObj == nullptr || activeObj->transform == nullptr) {
                continue;
            }


            if (cam.isInRenderView(activeObj->transform->getPosition())) {
                Vector2 drawnAt = activeObj->transform->getPosition() - cam.transform->getPosition();

                //test collision before updating position
                if (activeObj->collider != nullptr) {
                    activeObj->collider->center = drawnAt;

                    //collision detection
                    for (auto comparingObj: GObject::activeObjects) {
                        //if same obj or comparing is null or both static, save time
                        if (comparingObj == activeObj || comparingObj->collider == nullptr || activeObj->collider->
                            isStatic && comparingObj->collider->isStatic || !!cam.isInRenderView(
                                comparingObj->transform->getPosition()))
                            continue;

                        comparingObj->collider->center =
                                comparingObj->transform->getPosition() - cam.transform->getPosition();

                        GObject *nonStaticObject = comparingObj->collider->isStatic ? activeObj : comparingObj;
                        GObject *staticObject = nonStaticObject == activeObj ? comparingObj : activeObj;

                        Vector2 push;
                        if (nonStaticObject->collider->isColliding(*staticObject->collider, push)) {
                            Transform *trans = nonStaticObject->transform;
                            trans->setPosition(trans->getPosition() + push);

                            //send a notification
                            nonStaticObject->onCollision(staticObject->collider);
                            staticObject->onCollision(nonStaticObject->collider);
                        }
                    }

                    activeObj->collider->drawColliderBox(myApp.renderer, drawnAt);
                }


                activeObj->texture->render(drawTool, drawnAt);
                activeObj->onRender(drawnAt);
            }


            //once we render we can now check for collision


            if (elapsed > UPDATE_DELAY_MS) {
                //Call the update
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
