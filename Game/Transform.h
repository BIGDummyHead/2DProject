//
// Created by shawn on 5/16/2025.
//

#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <vector>

#include "../Vector2.h"


class Transform {

private:
    Transform* parentPtr = nullptr;

public:
    Vector2 position;
    Vector2 rotation;
    std::vector<Transform*> children;

    Transform* getParent();
    void setParent(Transform* parent);



};



#endif //TRANSFORM_H
