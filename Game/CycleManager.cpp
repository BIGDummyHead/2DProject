//
// Created by shawn on 7/6/2025.
//

#include "CycleManager.h"

#include <SDL_log.h>

AnimationCycle *CycleManager::getAnimationCycle(const std::string &name) {

    if(!animations.contains(name)) {
        SDL_LogWarn(SDL_LOG_PRIORITY_INFO, "You are trying to access an invalid animation from the getAnimationCycle function, please note that this may affect animations!");
        return nullptr;
    }

    return animations[name];
}

bool CycleManager::addAnimationCycle(const std::string &name, AnimationCycle *animation) {
    const bool cont = animations.contains(name);

    if(!cont) {
        animation->loop = true;
        animation->setLastCycle(mainCycle);
        animations[name] = animation;
    }

    return !cont;
}


