//
// Created by shawn on 7/2/2025.
//

#include "Player.h"



//Called once initialized
void Player::start() {
    idleSheet = new Sheet(drawTool->loadTexture(PLAYER_IDLE_FILE), 4,4);
    attackSheet = new Sheet(drawTool->loadTexture(PLAYER_ATTACK_FILE), 4,6);
    deathSheet = new Sheet(drawTool->loadTexture(PLAYER_DEATH_FILE), 4,11);
    moveSheet = new Sheet(drawTool->loadTexture(PLAYER_MOVE_FILE), 4,6);


    cycleManager = new CycleManager(idleSheet, 0,0);


    //Create the animations for each state here:
    auto* idleCenter = AnimationCycle::createCycleForRow(idleSheet, 0, 200);
    auto* idleLeft = AnimationCycle::createCycleForRow(idleSheet, 1, 200);
    auto* idleRight = AnimationCycle::createCycleForRow(idleSheet, 2, 200);
    auto* idleAway = AnimationCycle::createCycleForRow(idleSheet, 3, 200);

    auto* attackCenter = AnimationCycle::createCycleForRow(attackSheet, 0, 50);
    auto* attackLeft = AnimationCycle::createCycleForRow(attackSheet, 1, 50);
    auto* attackRight = AnimationCycle::createCycleForRow(attackSheet, 2, 50);
    auto* attackAway = AnimationCycle::createCycleForRow(attackSheet, 3, 50);

    auto* walkCenter = AnimationCycle::createCycleForRow(moveSheet, 0, 50);
    auto* walkLeft = AnimationCycle::createCycleForRow(moveSheet, 1, 50);
    auto* walkRight = AnimationCycle::createCycleForRow(moveSheet, 2, 50);
    auto* walkAway = AnimationCycle::createCycleForRow(moveSheet, 3, 50);

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

    AnimationCycle::ConditionalFunc conditioner = [this](const AnimationCycle* current) -> AnimationCycle* {
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

        //action = "idle";
        //lastDirection = "_right";
        const auto anim = cycleManager->getAnimationCycle(action + lastDirection);

        return anim;
    };


    cycleManager->mainCycle->addCondition("main_conditioner", conditioner);


    texture = cycleManager->mainCycle;


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

bool Player::testCondition(AnimationCycle *currentCycle) {

    auto mag = getMovement().magnitude();

    if(mag > 0) {
        std::cout << "HAHA" << std::endl;
    }

    return false;
}


void Player::animateMovement(const Vector2 &move, const double &magnitude) {

    if(cycleManager->mainCycle->isReady()) {
        if(auto* nextMove = cycleManager->mainCycle->moveNext()) {
            cycleManager->mainCycle = nextMove;
            texture = cycleManager->mainCycle->getPresentingSheet();

        }
    }

}

Vector2 Player::getMovement() {
    const double horiMovement = input::getMovement(Horizontal);
    const double vertMovement = input::getMovement(Vertical);

    const Vector2 relMovement {horiMovement, vertMovement};
    return  relMovement;
}



//Called every update
void Player::update() {
    Vector2 relMovement = getMovement();
    const double magnitude = relMovement.magnitude();



    if(magnitude > 0) { //the character is moving

    }
    else { //the character is not moving

    }
    animateMovement(relMovement, magnitude);
}

