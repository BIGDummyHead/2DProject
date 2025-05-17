#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "draw.h"
#include "init.h"
#include "input.h"
#include "Game/GObject.h"
#include "Game/Sheet.h"

SDL_Texture* createPlayer(draw& tool, Vector2& position) {

    SDL_Texture* playerText = tool.loadTexture(R"(assets\player\Idle\Character_Idle.png)");

    position.x = 100;
    position.y = 100;

    return  playerText;
}

[[noreturn]] int main() {

    App myApp;
    myApp.name = "My Game!";

    init::initSDL(myApp);

    draw drawTool(&myApp);

    GObject playerObject("Custom Name");
    Sheet sheet(drawTool.loadTexture(R"(assets\player\Move\Character_Move.png)"), 4, 6);
    sheet.scale = Vector2(4, 4);
    playerObject.texture = &sheet;
    playerObject.transform->position = Vector2(100, 100);


    while(true) {
        //game loop
        drawTool.prepareScene();

        input::doInput();

        Vector2 scale(1,1);
        //drawTool.blitSheet(playerRender, 4, 4, 2, 2, pos, scale);

        if(playerObject.texture != nullptr) {
            playerObject.texture->render(drawTool, playerObject.transform->position);

            auto* texSheet = (Sheet*)playerObject.texture;

            if(texSheet->getCurrentCol() == 5  && texSheet->getCurrentRow() != 3) {
                texSheet->setCol(0);
                texSheet->moveRowDown();
            }
            else if(texSheet->getCurrentCol() == 5 && texSheet->getCurrentRow() == 3) {
                texSheet->setCol(0);
                texSheet->setRow(0);
            }
            else {
                texSheet->moveColRight();

            }


        }

        drawTool.presentScene();

        SDL_Delay(50);
    }

}
