//
// Created by shawn on 5/24/2025.
//

#include "Raycaster.h"
#include <float.h>
#include <bits/stl_algo.h>

bool Raycaster::lineIntersectsRect(const Vector2& rayStart, const Vector2& rayEnd, const GObject* obj) {

      //some base case statement
      if(rayStart.x == rayEnd.x && rayStart.y == rayEnd.y)
            return false;

      const Vector2 b = obj->transform->getPosition();

      if(rayStart.x == b.x && rayStart.y == b.y)
            return false; //calling object

      //a to c
      const Vector2 ac(rayEnd.x - rayStart.x, rayEnd.y - rayStart.y);

      //a to b
      const Vector2 ab(b.x - rayStart.x, b.y - rayStart.y);

      //calculate the projection scalar for this object
      double projectionScalar = (ab.x * ac.x + ab.y * ac.y) / (pow(ac.x, 2.0) + pow(ac.y, 2.0));
      projectionScalar = std::max(0.0, std::min(1.0, projectionScalar));

      //find the closest point that this may interesect, allows for thresholds
      const Vector2 closestPoint(rayStart.x + projectionScalar * (rayEnd.x - rayStart.x), rayStart.y + projectionScalar * (rayEnd.y - rayStart.y));

      //grab the collider size
      const Vector2 colliderSize = obj->collider->getSize();
      const double halfWidth =colliderSize.x / 2;
      const double halfHeight = colliderSize.y / 2;

      //check if the closest point intersects with b
      const bool insideX = (closestPoint.x >= b.x - halfWidth) && (closestPoint.x <= b.x + halfWidth);
      const bool insideY = (closestPoint.y >= b.y - halfHeight) && (closestPoint.y <= b.y + halfHeight);

      //true if these both intersect
      return insideX && insideY;
}


//Returns true if something intersected
bool Raycaster::cast(const Vector2 &position, const Vector2 &angle, const double& distance, RayInfo *rayInformation) {

      const double endX = position.x + angle.x * distance;
      const double endY = position.y + angle.y * distance;

      const SDL_Rect rayEndPoint(static_cast<int>(endX), static_cast<int>(endY), 1, 1);


      auto closestDistance = DBL_MAX;
      rayInformation->castFrom = position;

      for(auto* obj : GObject::activeObjects) {

            if(obj->collider == nullptr)
                  return false;

            const Vector2 objPosition = obj->transform->getPosition();


            //it has hit something
            if(lineIntersectsRect(position, Vector2(rayEndPoint.x, rayEndPoint.y), obj)) {
                  const double objDistanceToRay = position.distance(objPosition);


                  //assume if 0, it is the caller
                  if(objDistanceToRay < closestDistance) {
                        //select this as the clsoest item now
                        closestDistance = objDistanceToRay;
                        rayInformation->collider = obj->collider;
                        rayInformation->gameObjectHit = obj;
                        rayInformation->positionHit = objPosition;
                  }
            }

      }

      return rayInformation->gameObjectHit != nullptr; //we have to have set something
}
