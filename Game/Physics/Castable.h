//
// Created by shawn on 6/23/2025.
//

#ifndef CASTABLE_H
#define CASTABLE_H
#include "../Collider.h"
#include "../../Vector2.h"

#endif //CASTABLE_H

struct Castable {
    Vector2 position;
    Collider* collider = nullptr;
};