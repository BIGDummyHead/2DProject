//
// Created by shawn on 7/3/2025.
//

#include "SheetAnimator.h"

#include <SDL_log.h>

bool SheetAnimator::setAnimationDelay(const int &animDelayMS, const bool& resetIfTrue) {
    const bool canSet = animDelayMS > 0;
    this->animationDelayMs = canSet ? animDelayMS : animationDelayMs;

    if(canSet && resetIfTrue)
        resetAnimateTimer();

    return canSet;
}

int SheetAnimator::getAnimationDelayMS() const {
    return animationDelayMs;
}

bool SheetAnimator::shouldAnimate() const {
    const auto now = timeNow();
    const auto elapsedMs =
        std::chrono::duration_cast<std::chrono::milliseconds>
    (now - lastAnimationTime).count();

    return  elapsedMs >= animationDelayMs;
}

void SheetAnimator::resetAnimateTimer() {
    lastAnimationTime = timeNow();
}

std::chrono::steady_clock::time_point SheetAnimator::timeNow() {
    return std::chrono::steady_clock::now();
}






