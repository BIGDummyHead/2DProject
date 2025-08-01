//
// Created by shawn on 7/2/2025.
//

#include "Player.h"

#include "../../Game/LightSourceComponent.h"


//Called once initialized
void Player::start() {

    auto appSettings = *App::getInstance()->getSettings();
    Vector2 center = {appSettings.windowDimensions.x / 2, appSettings.windowDimensions.y / 2};
    LightSource lSource(center, 300, 100, 0, 360);
    lSource.createRayCastedShadowing = false;

    //addComponent<LightSourceComponent>()->setSource(&lSource);


    auto* drawTool = Draw::getInstance();
    idleSheet = new Sheet(drawTool->loadTexture(PLAYER_IDLE_FILE), 4,4);
    attackSheet = new Sheet(drawTool->loadTexture(PLAYER_ATTACK_FILE), 4,6);
    deathSheet = new Sheet(drawTool->loadTexture(PLAYER_DEATH_FILE), 4,11);
    moveSheet = new Sheet(drawTool->loadTexture(PLAYER_MOVE_FILE), 4,6);


    cycleManager = new CycleManager();


    constexpr auto WALKING_DELAY = 100;
    constexpr auto ATTACKING_DELAY = 100;
    constexpr auto IDLE_DELAY = 200;
    constexpr auto DEATH_DELAY = 50;

    //Create the animations for each state here:
    auto* idleCenter = AnimationCycle::createCycleForRow(idleSheet, 0, IDLE_DELAY);
    auto* idleLeft = AnimationCycle::createCycleForRow(idleSheet, 1, IDLE_DELAY);
    auto* idleRight = AnimationCycle::createCycleForRow(idleSheet, 2, IDLE_DELAY);
    auto* idleAway = AnimationCycle::createCycleForRow(idleSheet, 3, IDLE_DELAY);

    auto* attackCenter = AnimationCycle::createCycleForRow(attackSheet, 0, ATTACKING_DELAY);
    auto* attackLeft = AnimationCycle::createCycleForRow(attackSheet, 1, ATTACKING_DELAY);
    auto* attackRight = AnimationCycle::createCycleForRow(attackSheet, 2, ATTACKING_DELAY);
    auto* attackAway = AnimationCycle::createCycleForRow(attackSheet, 3, ATTACKING_DELAY);

    auto* walkCenter = AnimationCycle::createCycleForRow(moveSheet, 0, WALKING_DELAY);
    auto* walkLeft = AnimationCycle::createCycleForRow(moveSheet, 1, WALKING_DELAY);
    auto* walkRight = AnimationCycle::createCycleForRow(moveSheet, 2, WALKING_DELAY);
    auto* walkAway = AnimationCycle::createCycleForRow(moveSheet, 3, WALKING_DELAY);

    auto* deathAnimation = AnimationCycle::createCycleForRow(deathSheet, 0, DEATH_DELAY);
    deathAnimation->loop = false;

    cycleManager->addAnimationCycle("idling_center", idleCenter);
    cycleManager->addAnimationCycle("idling_left", idleLeft);
    cycleManager->addAnimationCycle("idling_right", idleRight);
    cycleManager->addAnimationCycle("idling_away", idleAway);

    cycleManager->addAnimationCycle("attack_center", attackCenter);
    cycleManager->addAnimationCycle("attack_left", attackLeft);
    cycleManager->addAnimationCycle("attack_right", attackRight);
    cycleManager->addAnimationCycle("attack_away", attackAway);

    cycleManager->addAnimationCycle("walk_center", walkCenter);
    cycleManager->addAnimationCycle("walk_left", walkLeft);
    cycleManager->addAnimationCycle("walk_right", walkRight);
    cycleManager->addAnimationCycle("walk_away", walkAway);

    cycleManager->addAnimationCycle("dead", deathAnimation);

    lastDirection = "_center";
    CycleManager::ConditionalFunc conditioner = [this](const CycleManager* manager) -> AnimationCycle* {

        if(isDead) {
            return cycleManager->getAnimationCycle("dead");
        }

        const auto movement = getMovement();

        const auto mag = movement.magnitude();

        std::string action = mag != 0 ? "walk" : "idling";

        //other things like attacking and other movements should go here!

        if(movement.x > 0) {
            lastDirection = "_right";
        }
        else if(movement.x < 0) {
            lastDirection = "_left";
        }

        //vertical movements should override
        if(movement.y > 0) {
            lastDirection = "_away";
        }
        else if(movement.y < 0) {
            //this code is repeated above as _direction is def
            lastDirection = "_center";
        }

        const auto leftState = Input::getMouseInputState(Left);
        if(leftState == Held || leftState == Down) {
            action = "attack";
        }

        //action = "idle";
        //lastDirection = "_right";
        const auto anim = cycleManager->getAnimationCycle(action + lastDirection);

        return anim;
    };


    //cycleManager->mainCycle->addCondition("main_conditioner", conditioner);
    cycleManager->addCondition("main_conditioner", conditioner);

    texture = cycleManager->findCondition();


    //must come after setting texture
    //collider = automaticCollider(false, true);
}



void Player::doMovement(Vector2 relativeMove) const {
    relativeMove *= stats->movementMultiplier;

    // Temporarily update the player's position
    transform->setPosition(transform->getPosition() + relativeMove);


    // After collision resolution, update the camera's position
    if (cam != nullptr) {
        cam->transform->setPosition(transform->getPosition() - initSpawn);
    }

}

void Player::animateMovement(const Vector2 &move, const double &magnitude) {

    /*if(cycleManager->mainCycle->isReady()) {
        if(auto* nextMove = cycleManager->mainCycle->moveNext()) {
            cycleManager->mainCycle = nextMove;
            texture = cycleManager->mainCycle->getPresentingSheet();

        }
    }*/

    texture = cycleManager->getAnimatedTexture();
}

Vector2 Player::getMovement() {
    const double horiMovement = Input::getMovement(Horizontal);
    const double vertMovement = Input::getMovement(Vertical);

    const Vector2 relMovement {horiMovement, vertMovement};
    return  relMovement;
}



//Called every update
void Player::update() {

    Vector2 relMovement = getMovement();
    const double magnitude = relMovement.magnitude();

    if(!isDead && Input::getMouseInputState(Right) == Down) {
        isDead = true;
    }


    if(magnitude > 0) { //the character is moving

    }
    else { //the character is not moving

    }

    if(canMove) {
        relMovement.y = relMovement.y * -1;
        animateMovement(relMovement, magnitude);
        doMovement(relMovement);
    }
}

