//
// Created by shawn on 7/6/2025.
//

#include "CycleManager.h"

AnimationCycle *CycleManager::getAnimationCycle(const std::string &name) {
    return animations.contains(name) ? animations[name] : nullptr;;
}

bool CycleManager::addAnimationCycle(const std::string &name, AnimationCycle *animation) {
    const bool cont = animations.contains(name);

    if(!cont) {
        animation->loop = false;
        animation->setLastCycle(mainCycle);
        animations[name] = animation;

    }

    return !cont;
}


