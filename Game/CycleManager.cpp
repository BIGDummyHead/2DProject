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
        //animation->loop = true;
        //animation->setLastCycle(mainCycle);
        animations[name] = animation;
    }

    return !cont;
}

AnimationCycle *CycleManager::findCondition() const {

    for (const auto &val: *conditions | std::views::values) {
        const Branch *condBranch = val;

        if (!condBranch) {
            SDL_LogWarn(SDL_LOG_CATEGORY_ERROR, "Invalid condition resulted in a caught null ptr.");
            continue;
        }

        //if this condition is true
        if (const auto newCyc = condBranch->condition(this)) {
            return newCyc;
        }
    }

    return nullptr;
}

AnimationCycle *CycleManager::getCondition(const std::string &identifier) const {
    return nullptr;
    //return conditions->contains(identifier) ? conditions->operator[](identifier)->newCycle : nullptr;
}


void CycleManager::addCondition(const std::string& identifier, const ConditionalFunc &condition) const {
    if (conditions->contains(identifier)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     ("A key with the name '" + identifier + "' already exist in your conditions branch").c_str());
        return;
    }

    auto *b = new Branch();
    b->condition = condition;

    conditions->operator[](identifier) = b;
}

Sheet *CycleManager::getAnimatedTexture() {

   const auto foundCondition = findCondition();

    if(foundCondition == nullptr) {
        return nullptr;
    }

    //The same original condition that was found + the animating one is ready
    if(foundCondition == theOriginalCondition && animatingCondition && animatingCondition->isReady()) {
        //move the animation forward
        const auto next = animatingCondition->moveNext();

        if(next) {
            animatingCondition = animatingCondition->moveNext();
        }
    }
    else if(foundCondition != theOriginalCondition) {
        //a new condition is now present, update what is being animated and the actual condtion that was found
        animatingCondition = theOriginalCondition = foundCondition;
    }

    return animatingCondition == nullptr ? nullptr : animatingCondition->getPresentingSheet();
}







