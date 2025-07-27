//
// Created by shawn on 7/2/2025.
//

#ifndef PLAYER_H
#define PLAYER_H
#include "PlayerStats.h"
#include "../../Input.h"
#include "../../Game/AnimationCycle.h"
#include "../../Game/Camera.h"
#include "../../Game/CycleManager.h"
#include "../../Game/GameObject.h"


class Player final : public GameObject {
private:
    std::string PLAYER_ASSETS_FOLDER = R"(assets/player/)";
    std::string PLAYER_DEATH_FILE = PLAYER_ASSETS_FOLDER + "Death/Character_Death.png";
    std::string PLAYER_ATTACK_FILE = PLAYER_ASSETS_FOLDER + "Attack/Character_Attack.png";
    std::string PLAYER_IDLE_FILE = PLAYER_ASSETS_FOLDER + "Idle/Character_Idle.png";
    std::string PLAYER_MOVE_FILE = PLAYER_ASSETS_FOLDER + "Move/Character_Move.png";

    std::string lastDirection;

    Camera* cam;
    PlayerStats* stats;

    static constexpr int ANIMATION_DELAY_MS = 100;
    CycleManager* cycleManager = nullptr;

    bool isDead = false;

public:

    bool canMove = true;

    Sheet* idleSheet = nullptr;
    Sheet* deathSheet = nullptr;
    Sheet* attackSheet = nullptr;
    Sheet* moveSheet = nullptr;

    Vector2 initSpawn;


    void update() override;
    void start() override;

    static Vector2 getMovement();

    bool testCondition(AnimationCycle* currentCycle);

    void doMovement(Vector2 relativeMove) const;
    void animateMovement(const Vector2& move, const double& magnitude);


    explicit Player(const Vector2& spawn, PlayerStats* pStats = nullptr) :
    GameObject("Player"),
    initSpawn(spawn) {
        cam = Camera::mainCamera;
        stats = pStats ? pStats : new PlayerStats();
        transform->setPosition(spawn);


    }



};



#endif //PLAYER_H
