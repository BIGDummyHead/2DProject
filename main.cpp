#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "draw.h"
#include "init.h"
#include "input.h"

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

    Vector2 pos;
    SDL_Texture* playerRender = createPlayer(drawTool, pos);


    while(true) {
        //game loop
        drawTool.prepareScene();

        input::doInput();

        drawTool.blitSheet(playerRender, 4, 4, 2, 2, pos, 3);

        drawTool.presentScene();

        SDL_Delay(16);
    }

}
