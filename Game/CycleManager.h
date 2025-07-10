//
// Created by shawn on 7/6/2025.
//

#ifndef CYCLEMANAGER_H
#define CYCLEMANAGER_H

#include <unordered_map>
#include <string>

#include "AnimationCycle.h"

class CycleManager {
private:
    std::unordered_map<std::string, AnimationCycle*> animations;

public:
    //The cycle to add upon via conditions
    AnimationCycle* mainCycle;

    explicit CycleManager(Sheet* defaultTexture,
        const int& row,
        const int& col,
        const int& timeTillNextAnimMs = 0) {

        mainCycle =
            new AnimationCycle(defaultTexture, row, col, timeTillNextAnimMs);
        mainCycle->loop = true;
        mainCycle->placeHolderSheet = true;
    }

    explicit CycleManager(Sheet* defaultSheet, const int& timeTillNextAnimMs = 0)
    : CycleManager(defaultSheet, defaultSheet->getCurrentRow(), defaultSheet->getCurrentCol(), timeTillNextAnimMs) {

    }

    AnimationCycle* getAnimationCycle(const std::string& name);
    bool addAnimationCycle(const std::string& name, AnimationCycle* animation);
};



#endif //CYCLEMANAGER_H
