//
// Created by shawn on 7/4/2025.
//

#ifndef ANIMATIONCYCLE_H
#define ANIMATIONCYCLE_H
#include <chrono>
#include <bits/stdc++.h>
#include "Sheet.h"

//COde:
class AnimationCycle final : public Texture {

public:
    typedef std::function<AnimationCycle* (const AnimationCycle* anim)> ConditionalFunc;

private:
    struct Branch {
    public:
        ConditionalFunc condition;
    };

    int row = 0;
    int column = 0;
    static constexpr int DEFAULT_TIME_MS = 95;
    std::chrono::steady_clock::time_point animationTime;
    std::chrono::steady_clock::time_point timeOfNextAnimation;
    AnimationCycle *next = nullptr;
    AnimationCycle *start;

    std::unordered_map<std::string, Branch *> *conditions;

    const bool isFirstAnimationCycle;

    //Finds the best condition that is true. If false, returns nullptr
    [[nodiscard]] AnimationCycle* findCondition() const;

    AnimationCycle(Sheet *animatingTexture, const int &row, const int &column, const bool &isFirst,
                   const int &timeTillNextMs, AnimationCycle *start)
        : isFirstAnimationCycle(isFirst) {
        this->start = start;
        animating = animatingTexture;
        setAnimRow(row);
        setAnimCol(column);
        this->delayOfNextAnimation = timeTillNextMs;

        //If this is the first item of the branch then create a newconditions map
        //if not. use the original
        conditions = isFirst ? new std::unordered_map<std::string, Branch *> : start->conditions;

    }

public:


    //The next animation cycle
    //The time until next animation
    int delayOfNextAnimation = DEFAULT_TIME_MS;

    Sheet *animating;

    //When this reaches the end of the cycle, restart the animation
    bool loop = true;


    //Creates a FIRST animation cycle, to create further cycles refer to createNextCylce
    AnimationCycle(Sheet *animatingTexture, const int &row, const int &column,
                   const int &timeTillNextMs): AnimationCycle(
        animatingTexture, row, column, true, timeTillNextMs, this) {
    }


    //True when the next animation cycle is null.
    [[nodiscard]] bool isEndOfCycle() const;

    //Sets the row for animation, disallows going beyond the sheets indexing
    void setAnimRow(const int &row);

    //Sets the column for animation, disallows going beyond the sheets indexing
    void setAnimCol(const int &col);

    //Get the current row for this animation
    [[nodiscard]] int getRow() const;

    //Get the current column for this animation
    [[nodiscard]] int getCol() const;

    //Create a new cycle based on this sheet or a new one. Sets the next animation cycle
    AnimationCycle *createNextCycle(const int &row, const int &column, const int &timeTillNextMs,
                                    Sheet *newSheet = nullptr);

    //Create a new cycle based on this sheet or a new one. Sets the next animation cycle. Using the same timing
    AnimationCycle *createNextCycle(const int &row, const int &column, Sheet *newSheet = nullptr);

    //Returns the time now
    static std::chrono::steady_clock::time_point now();

    //Prepares and gets the sheet that needs to be presented.
    Sheet* getPresentingSheet();

    //Determines if you should move to the next animation cycle
    [[nodiscard]] bool isReady() const;

    //Move to the next animation, does not care if it is ready or not
    [[nodiscard]] AnimationCycle *moveNext() const;

    //Moves to the next animation when the isReady() function returns true. Can return nullptr if not ready.
    [[nodiscard]] AnimationCycle *moveNextWhenReady() const;

    //Adds a condition, if the condition is true, switches to the new condition.
    //It is important to note that conditions are applied to literally every cycle created
    void addCondition(
        const std::string &identifier,
        const ConditionalFunc &condition) const;

    [[nodiscard]] AnimationCycle* getCondition(const std::string& identifier) const;

    void render(const draw &drawTool, Vector2 where) override;

    //Returns a cycle from the beginning.
    [[nodiscard]] AnimationCycle* getAt(const int& index) const;

    void setNextCycle(AnimationCycle* cycle);
    void setLastCycle(AnimationCycle* cycle);

    static AnimationCycle* createCycleForRow(Sheet* texture, const int& row, const int& timePerFrame);
    static AnimationCycle* createCycleForCol(Sheet* texture, const int& col, const int& timePerFrame);
};

#endif //ANIMATIONCYCLE_H
