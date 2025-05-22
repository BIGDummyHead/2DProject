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
    Vector2 position;
    Vector2 rotation;
    Vector2 drawnPosition;
    bool hasDrawnPosition = false;

public:

    std::vector<Transform*> children;

    void setParent(Transform* parent);
    [[nodiscard]] Transform* getParent() const;

    void setPosition(const Vector2& pos);
    [[nodiscard]] Vector2 getPosition() const;

    void setRotation(const Vector2& rot);
    [[nodiscard]] Vector2 getRotation() const;



};



#endif //TRANSFORM_H
