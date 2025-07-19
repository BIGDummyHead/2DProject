#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "draw.h"
#include "init.h"
#include "input.h"
#include "Game/GObject.h"
#include "Game/Sheet.h"
#include <chrono>

#include "UiObjectFont.h"
#include "UiObjectFont.h"
#include "Audio/AudioManager.h"
#include "Game/Camera.h"
#include "Game/Collider.h"
#include "Game/Scene.h"
#include "Game/Physics/Raycaster.h"
#include "IP_Games/Knight/Knight_Game.h"


#include "SDL_ttf/include/SDL_ttf.h"
#include "Settings/Parser.h"

Scene *getKnightGame() {
    auto *game = new Knight_Game();
    return game;
}

int main() {
    auto *device = AudioManager::getDefaultDevice();

    auto *sound = new Sound("pac.wav");
    auto *exmpWav = new Sound("myfile.wav");

    exmpWav->setVolume(.2f);
    exmpWav->loop = false;



    while(!exmpWav->stopped()) {

    }


    return 0;
    //Initialize the application
    App myApp;
    myApp.name = "Knight Game";
    init::initSDL(myApp);

    //create a drawtool from application
    draw drawTool(&myApp);

    //Center of the Screen, can be used for rendering

    //Create a camera to use for later, this will control scrolling
    const Camera cam(Vector2(SCREEN_WIDTH + 150, SCREEN_HEIGHT + 250), Vector2{0, 0});

    const SceneInformation sceneInfo{&myApp, &drawTool};

    //Scene::loadScene("Main Scene", sceneInfo);

    Scene *kGame = getKnightGame();

    Scene::loadScene(kGame, sceneInfo);

    //    Scene::loadFirstAvalScene(sceneInfo);


    Uint32 lastTick = -1;
    while (true) {
        const Uint32 thisTick = SDL_GetTicks();
        if (thisTick - lastTick < 1000 / FPS) {
            continue;
        }
        lastTick = thisTick;

        //Prepare the scene for rendering
        drawTool.prepareScene();

        //Poll for inputs, updates the input sectors
        input::pollInput();


        //Loop over active scene objects.
        for (auto activeObj: GObject::activeObjects) {
            //Ensure this object is not null for drawing
            if (activeObj == nullptr || activeObj->transform == nullptr) {
                continue;
            }

            //important to only call this once
            const Vector2 vel = activeObj->transform->getVelocity();

            //check if the object is even in the render view
            if (cam.isInRenderView(activeObj->transform->getPosition())) {
                //determine where to draw this object
                Vector2 drawnAt = activeObj->transform->getPosition() - cam.transform->getPosition();

                //test collision before updating position
                if (activeObj->collider != nullptr) {
                    activeObj->collider->center = drawnAt;

                    //we have to loop through all the active objects again and make sure there is not collision
                    for (auto comparingObj: GObject::activeObjects) {
                        //check if same object, if the collider is null, both collider's are static, and or the other is in the render view, if yes continue.
                        if (comparingObj == activeObj || comparingObj->collider == nullptr || activeObj->collider->
                            isStatic && comparingObj->collider->isStatic || !cam.isInRenderView(
                                comparingObj->transform->getPosition()))
                            continue;


                        //determine the center of this collider
                        comparingObj->collider->center =
                                comparingObj->transform->getPosition() - cam.transform->getPosition();


                        //dynamic or heaviest
                        GObject *left = nullptr;

                        //static or lightest
                        GObject *right = nullptr;

                        if (comparingObj->collider->isStatic == false && activeObj->collider->isStatic == false) {
                            //both are dynamic

                            //choose the fastest and heaviest object

                            //get the last stored velocity, avoids the recalcuation step
                            const Vector2 comparingVelocity = comparingObj->transform->getVelocity(true);
                            const double comparingWeight = comparingObj->collider->mass;
                            const double myWeight = activeObj->collider->mass;

                            left = (comparingVelocity.magnitude() * comparingWeight) > (vel.magnitude() * myWeight)
                                       ? comparingObj
                                       : activeObj;
                            right = left == activeObj ? comparingObj : activeObj;
                        } else {
                            //one is static and the other is dynamic
                            //static object
                            left = comparingObj->collider->isStatic ? activeObj : comparingObj;

                            //dynamic object
                            right = left == activeObj ? comparingObj : activeObj;
                        }

                        Vector2 push; //check for collision
                        if (left->collider->isColliding(*right->collider, push)) {
                            if (left->collider->isStatic == false && right->collider->isStatic == false) {
                                Transform *rightTrans = right->transform;

                                //fix for object clipping when one was heavier.
                                rightTrans->setPosition(rightTrans->getPosition() - push / left->collider->mass);
                            }

                            //collision gives us back the amount that we need to push the nonStaticObject
                            Transform *trans = left->transform;
                            const Vector2 newLocation = trans->getPosition() + push;
                            trans->setPosition(newLocation);

                            //send a notification
                            right->onCollision(left->collider);
                            left->onCollision(right->collider);
                        }
                    }


                    //debug: draw the collider box so we can see it
                    activeObj->collider->drawColliderBox(myApp.renderer, drawnAt);
                }


                //render the object
                if (activeObj->texture != nullptr)
                    activeObj->texture->render(drawTool, drawnAt);

                activeObj->onRender(drawnAt);
            }

            activeObj->updateFrame();
        }


        //Lights:
        //TOOD: reimplement this code for later
        /*SDL_Texture *lightmap = drawTool.startLightMap();


        drawTool.drawLights();
        drawTool.endLightMap();


        // Set multiply (modulate) blending mode so black hides, white reveals
        SDL_SetTextureBlendMode(lightmap, SDL_BLENDMODE_MOD);
        SDL_RenderCopy(myApp.renderer, lightmap, nullptr, nullptr);*/


        //Implementation of a texxt component
        /*
        SDL_Color foreGround(255, 255, 255, 255);
        UiFont font(R"(assets\fonts\font.ttf)", foreGround);
        SDL_Texture* text = drawTool.createTextTexture(font, "Hello Peyton");

        SDL_Rect textDest(100, 100, 500, 300);
        SDL_RenderCopy(drawTool.getApp().renderer, text, nullptr, &textDest);
        */

        //Render UI
        RayInfo rayInfo;
        const bool hasHitUI = Raycaster::castUI(&rayInfo);
        for (auto *uiObject: UiObject::getRegisteredUI()) {
            uiObject->render(); //always draw the object

            if (uiObject->collider == nullptr)
                continue; //go onto the next one

            //start any event calls here:
            if (uiObject == rayInfo.uiObjectHit) {
                if (!uiObject->isMouseFocused()) {
                    uiObject->setMouseIn();
                } else {
                    uiObject->whileMouseOver();
                }

                switch (input::getMouseInputState(Left)) {
                    case Down:
                        uiObject->onMouseClick();
                        break;
                    case Up:
                        uiObject->onMouseRelease();
                        break;
                    case Held:
                        uiObject->whileMouseDown();
                        break;
                    default:
                        break;
                }
            } else {
                if (uiObject->isMouseFocused()) {
                    uiObject->setMouseOut();
                } else {
                    uiObject->whileMouseOut();
                }
            }
        }

        //Once everything is ready to be rendered (what to render, collision, and final rendering) present the scene
        drawTool.presentScene();

        //Give a delay between render
        SDL_Delay(RENDER_DELAY_MS);
    }

    //quit out of everything!!!!
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

/* Old test code



class Test_Player final : public GObject {
private:
    Vector2 initSpawn;

    Sheet *textureSheet = new Sheet(drawTool->loadTexture(R"(assets\player\Idle\Character_Idle.png)"), 4, 4);
    Sheet *runningSheet = new Sheet(drawTool->loadTexture(R"(assets\player\Move\Character_Move.png)"), 4, 6);

    std::chrono::steady_clock::time_point lastAnimationTime = std::chrono::steady_clock::now();
    const int animationDelayMs = 95; // Delay in milliseconds

public:
    Camera *cam = nullptr;

    explicit Test_Player(draw *drawTool, const Vector2 &pos) : GObject(drawTool, "The Player"), initSpawn(pos) {
        textureSheet->scale *= 2.5;
        runningSheet->scale *= 2.5;

        texture = textureSheet;
        transform->setPosition(pos);

        textureSheet->render(*drawTool, Vector2());
        const Vector2 box = Collider::createBoxFromTexture(*textureSheet);
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

        texture = runningSheet;
        if (move.x > 0) {
            runningSheet->setRow(2);
        } else if (move.x < 0) {
            runningSheet->setRow(1);
        }

        if (move.y > 0) {
            runningSheet->setRow(0);
        } else if (move.y < 0) {
            runningSheet->setRow(3);
        }
        runningSheet->moveColRight(true);
    }

    void doMovement(Vector2 &move) const {
        move *= 4;

        // Temporarily update the player's position
        transform->setPosition(transform->getPosition() + move);


        // After collision resolution, update the camera's position
        if (cam != nullptr) {
            cam->transform->setPosition(transform->getPosition() - initSpawn);
        }
    }

    void update() override {
        Vector2 move(getHorizontalMove(), getVerticalMove());
        if (move.magnitude() != 0) {
            animateMovement(move);
            doMovement(move);
        } else {
            if (texture != textureSheet) {
                texture = textureSheet;
            }

            if (shouldAnimate()) {
                textureSheet->setRow(0);
                textureSheet->moveColRight(true);
            }
        }
    }
};


class Test_Light : public GObject {
public:
    Test_Light(draw *drawTool) : GObject(drawTool, "My Light") {
    }

    void update() override {
        const Vector2 position = input::getMousePosition();
        transform->setPosition(position);
    }

    void onRender(const Vector2 &drawnAt) override {
        drawTool->drawLine(Vector2{}, drawnAt + Camera::mainCamera->transform->getPosition());
    }
};

class Test_Component : public Component {
public:
    void start() override {
        std::cout << "This is a start message from test_component" << std::endl;
    }

    void update() override {
        /*std::cout << "This is a UPDATE message from test_component" << std::endl;*/
//}
/*

    void destroy() override {
        std::cout << "This is a destroyed message from test_component" << std::endl;
    }

    void testing() {
        std::cout << "Testing" << std::endl;
    }
};

class Test_UI final : public UiObject {

public:

    explicit Test_UI(draw* tool): UiObject(tool, new Texture(tool->loadTexture(R"(assets\player\Death\Character_Death.png)"))) {
        position = Vector2{SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
        collider = new Collider(100, 100);
    }
};

class Test_Scene final : public Scene {
public:
    Test_Scene() : Scene("Main Scene") {
    }

    void createDeathObject(SceneInformation sceneInfo, Vector2 where, double mass, const std::string &theName) {
        auto *obj = new GObject(sceneInfo.drawingTool, theName);
        auto *sheet = new Sheet(sceneInfo.drawingTool->loadTexture(R"(assets\player\Death\Character_Death.png)"), 4,
                                11);
        obj->collider = new Collider(25, 40);
        obj->collider->isStatic = false;
        obj->collider->mass = mass;
        obj->transform->setPosition(where + Vector2{-250, 0});
        sheet->scale *= 2.5;
        obj->texture = sheet;


        }


    //OBJECTS MUST BE ALLOCATED ON THE HEAP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    void onSceneLoad(const SceneInformation sceneInfo) override {
        if (Camera::mainCamera == nullptr)
            throw std::runtime_error("No camera was instantiated");cc

        const Vector2 center(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        createDeathObject(sceneInfo, center, 1, "left object");
        createDeathObject(sceneInfo, center + Vector2{500, 0}, 4, "right object");

        auto *light = new Test_Light(sceneInfo.drawingTool);

        /* Player Object */
/*
        auto *playerObject = new Test_Player(sceneInfo.drawingTool, center);
        playerObject->addComponent<Test_Component>();

        auto* testUI = new Test_UI(sceneInfo.drawingTool);

        //! Component Functionality Testing, working as of 06-14-2025
        /*auto* getTestComp = playerObject->getComponent<Test_Component>();

        if(getTestComp) {
            getTestComp->testing();
            std::cout << "Obj: " << getTestComp->getAttachedObject()->name << std::endl;
        }
        else {
            std::cout << "None" << std::endl;
        }

        playerObject->removeComponent<Test_Component>();

        getTestComp = playerObject->getComponent<Test_Component>();

        if(getTestComp) {
            getTestComp->testing();
            std::cout << "Obj: " << getTestComp->getAttachedObject()->name << std::endl;
        }
        else {
            std::cout << "None" << std::endl;
        }*/

/*
        playerObject->cam = Camera::mainCamera; // Hook the camera up to the GObject


        constexpr SDL_Color white(255,255,255,255);

        auto* tFont = new TextFont("Google", R"(assets\fonts\font.ttf)", white, true);
        // UiFont font(sceneInfo.drawingTool, tFont);

        auto* googleFont = TextFont::getFontByName("Google");

        auto* objFont = new UiObjectFont(sceneInfo.drawingTool, googleFont, "Hello world!");
        objFont->position = Vector2{100, 100};
    }
};





 */
