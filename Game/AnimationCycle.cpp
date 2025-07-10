//
// Created by shawn on 7/4/2025.
//

#include "AnimationCycle.h"

#include <SDL_log.h>

int AnimationCycle::getCol() const {
    return column;
}

int AnimationCycle::getRow() const {
    return row;
}

void AnimationCycle::setAnimCol(const int &col) {
    if (!animating) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Trying to read invalid pointer for setting anim cycle");
        return;
    }

    if (col < 0 || col > animating->totalCols()) {
        SDL_LogWarn(SDL_LOG_PRIORITY_WARN, "Setting col out of bounds.");
        return;
    }

    this->column = col;
}

void AnimationCycle::setAnimRow(const int &row) {
    if (!animating) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Trying to read invalid pointer for setting anim cycle");
        return;
    }

    if (row < 0 || row > animating->totalRows()) {
        SDL_LogWarn(SDL_LOG_PRIORITY_WARN, "Setting row out of bounds of.");
        return;
    }

    this->row = row;
}

bool AnimationCycle::isEndOfCycle() const {
    return this->next == nullptr || animating == nullptr;
}

AnimationCycle *AnimationCycle::createNextCycle(const int &row, const int &column, const int &timeTillNextMs,
                                                Sheet *newSheet) {
    next = new AnimationCycle(newSheet == nullptr ? animating : newSheet, row, column, false, timeTillNextMs, start);
    return next;
}

AnimationCycle *AnimationCycle::createNextCycle(const int &row, const int &column, Sheet *newSheet) {
    return createNextCycle(row, column, delayOfNextAnimation, newSheet);
}


std::chrono::steady_clock::time_point AnimationCycle::now() {
    return std::chrono::steady_clock::now();
}


bool AnimationCycle::isReady() const {
    if (isFirstAnimationCycle /*|| findCondition()*/)
        return true;

    //measure if the time of the last time this was animated(+ delay) is greater than now
    const std::chrono::milliseconds msAdd(delayOfNextAnimation);
    return now() > timeOfNextAnimation;
}

Sheet* AnimationCycle::getPresentingSheet() {

    if(placeHolderSheet) {
        const auto nextAnimatingSheet = moveNext();

        if(!nextAnimatingSheet) {
            return nullptr;
        }

        return nextAnimatingSheet->animating;
    }

    if (!animating) {
        SDL_LogWarn(SDL_LOG_CATEGORY_ERROR, "Cannot present, since animating was null");
        return nullptr;
    }

    //copy over this
    loadedTexture = animating->loadedTexture;
    animating->setRow(row);
    animating->setCol(column);
    return animating;
}



AnimationCycle *AnimationCycle::moveNext() const {

    const bool loopAnimation = start != nullptr ? start->loop : false;
    AnimationCycle *theNextCycle = !isEndOfCycle() ? next : (loopAnimation ? start : nullptr);

    if (theNextCycle) {
        theNextCycle->timeOfNextAnimation = now() + std::chrono::milliseconds(delayOfNextAnimation);
    }

    //may return a null pointer
    return theNextCycle;
}

AnimationCycle *AnimationCycle::moveNextWhenReady() const {
    return isReady() ? moveNext() : nullptr;
}





void AnimationCycle::render(const draw &drawTool, Vector2 where) {
    if(!animating) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "No animation for render.");
        return;
    }
    animating->render(drawTool, where);
}


AnimationCycle *AnimationCycle::getAt(const int &index) const {
    AnimationCycle* starting = this->start;

    if(!starting || index == 0)
        return starting;

    int i = 1;
    do {
        starting = starting->next;
        i++;
    }while (i < index && starting != nullptr);

    return starting;
}

void AnimationCycle::setNextCycle(AnimationCycle *cycle) {
    this->next = cycle;
}
void AnimationCycle::setLastCycle(AnimationCycle *cycle) {
    auto* now = this;
    do {
        if(!now->next) {
            now->next = cycle;
            break;
        }
        now = now->next; //move forward
    }while (true);
}


AnimationCycle *AnimationCycle::createCycleForCol(Sheet* texture, const int &col, const int &timePerFrame) {

    const int totalRows = texture->totalRows();

    AnimationCycle* current = nullptr;
    for(int i = 0; i < totalRows; i++) {

        if(!current) {
            auto* cycle =
            new AnimationCycle(texture, i, col, timePerFrame);

            current = cycle;
            continue;
        }

        current->createNextCycle(i, col, timePerFrame);
        current = current->next;
    }

    return current ? current->start : nullptr;
}


AnimationCycle *AnimationCycle::createCycleForRow(Sheet* texture, const int &row, const int &timePerFrame) {
    const int totalCols = texture->totalCols();


    AnimationCycle* current = nullptr;

    for(int i = 0; i < totalCols; i++) {


        if(!current) {
            auto* cycle =
            new AnimationCycle(texture, row, i, timePerFrame);
            current = cycle;
            continue;
        }

        current->createNextCycle(row, i, timePerFrame);
        current = current->next;
    }

    return current ? current->start : nullptr;
}
