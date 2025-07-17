//
// Created by shawn on 7/6/2025.
//

#ifndef CYCLEMANAGER_H
#define CYCLEMANAGER_H

#include <unordered_map>
#include <string>

#include "AnimationCycle.h"

class CycleManager {

public:
    typedef std::function<AnimationCycle* (const CycleManager* manager)> ConditionalFunc;

private:
    std::unordered_map<std::string, AnimationCycle*> animations;
    struct Branch {
    public:
        ConditionalFunc condition;
    };

    std::unordered_map<std::string, Branch *> *conditions;

    AnimationCycle* theOriginalCondition = nullptr;
    AnimationCycle* animatingCondition = nullptr;
public:
    //The cycle to add upon via conditions
    //AnimationCycle* mainCycle;

    explicit CycleManager()  {

        conditions = new std::unordered_map<std::string, Branch *>;
        /*  mainCycle =
            new AnimationCycle(defaultTexture, row, col, timeTillNextAnimMs);
        mainCycle->loop = true;
        mainCycle->placeHolderSheet = true; */
    }

    AnimationCycle* getAnimationCycle(const std::string& name);
    bool addAnimationCycle(const std::string& name, AnimationCycle* animation);


    //Finds the best condition that is true. If false, returns nullptr
    [[nodiscard]] AnimationCycle* findCondition() const;
    [[nodiscard]] AnimationCycle* getCondition(const std::string& identifier) const;

    //Adds a condition, if the condition is true, switches to the new condition.
    //It is important to note that conditions are applied to literally every cycle created
    void addCondition(
        const std::string& identifier,
        const ConditionalFunc &condition) const;

    Sheet* getAnimatedTexture();
};



#endif //CYCLEMANAGER_H
