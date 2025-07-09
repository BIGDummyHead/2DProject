//
// Created by shawn on 7/3/2025.
//

#ifndef SHEETANIMATOR_H
#define SHEETANIMATOR_H
#include <chrono>

#include "Sheet.h"


//helps animate a sheet
class SheetAnimator {
private:
    std::chrono::steady_clock::time_point lastAnimationTime = std::chrono::steady_clock::now();
    int animationDelayMs = 95; // Delay in milliseconds
public:
    explicit SheetAnimator(/*Sheet* sheet,*/ const int& animDelay) {
        /*if(!sheet) {
            throw std::runtime_error("The sheet provided for animation was null");
        }
        this->sheet = sheet;*/

        if(!setAnimationDelay(animDelay)) {
            throw std::runtime_error("The sheet you are trying to animate must have a correct animation delay");
        }
    }

    //Returns true when the animation delay was successfully set/updated
    bool setAnimationDelay(const int& animDelayMS, const bool& resetIfTrue = false);
    //Get the delay for animation in Ms
    [[nodiscard]] int getAnimationDelayMS() const;
    //Determine if the time since the last animation has passed and this is ready to animate
    [[nodiscard]] bool shouldAnimate() const;
    //Reset the last time animated to be now
    void resetAnimateTimer();
    static std::chrono::steady_clock::time_point timeNow();
    //const auto now = std::chrono::steady_clock::now();

};



#endif //SHEETANIMATOR_H
