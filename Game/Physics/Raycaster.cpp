//
// Created by shawn on 5/24/2025.
//

#include "Raycaster.h"
#include <float.h>
#include <bits/stl_algo.h>

#include "../Camera.h"

bool Raycaster::lineIntersectsRect(const Vector2& rayStart, const Vector2& rEnd, const GObject* obj) {

      Vector2 rayEnd = rEnd;
      rayEnd.x = static_cast<int>(rayEnd.x);
      rayEnd.y = static_cast<int>(rayEnd.y);
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

      //check if the closest point intersects with b
      const bool insideX = (closestPoint.x >= b.x - colliderSize.x) && (closestPoint.x <= b.x + colliderSize.x);
      const bool insideY = (closestPoint.y >= b.y - colliderSize.y) && (closestPoint.y <= b.y + colliderSize.y);

      //true if these both intersect
      return insideX && insideY;
}

Vector2 Raycaster::createEndPoint(const Ray &ray) {

      const Vector2 normalized = ray.angle.normalize();

      Vector2 ret(
          ray.position.x + normalized.x * ray.distance,
          ray.position.y + normalized.y * ray.distance
      );

      return ret;
}



//Returns true if something intersected
bool Raycaster::cast(const Ray& ray, RayInfo *rayInformation) {

      const Vector2& rayEndPoint = createEndPoint(ray);

      //const SDL_Rect rayEndPoint(static_cast<int>(rayPoint.x), static_cast<int>(rayPoint.y), 1, 1);
      auto closestDistance = DBL_MAX;
      rayInformation->castFrom = ray.position;

      for(auto* obj : GObject::activeObjects) {

            //ignore anything without a collider.
            if(obj->collider == nullptr)
                  return false;

            const Vector2 objPosition = obj->transform->getPosition();

            //it has hit something
            if(lineIntersectsRect(ray.position, rayEndPoint, obj)) {
                  const double objDistanceToRay = ray.position.distance(objPosition);

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

void Raycaster::drawCast(const Ray &ray, SDL_Renderer* renderer, const RayInfo& rInfo) {

      //do not do anything if we cannot draw or determine
      if(renderer == nullptr || Camera::mainCamera == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Renderer or Camera::mainCamera is null pointer.");
            return;;
      }

      //determine if this was even hit
      const bool isHit = rInfo.gameObjectHit != nullptr; //has something there

      //get the position of the camera being used
      const Vector2 camPos = Camera::mainCamera->transform->getPosition();

      //determine where to draw the line
      Vector2 endPoint = createEndPoint(ray) - camPos;
      endPoint.x = isHit ? rInfo.gameObjectHit->transform->getPosition().x - camPos.x : endPoint.x;
      const Vector2 camStart = ray.position - camPos;

      //create a white line if it is not hit, green if it is
      const Uint8 r = isHit ? 0 : 255;
      const Uint8 b = isHit ? 0 : 255;

      SDL_SetRenderDrawColor(renderer, r, 255, b, 255);
      SDL_RenderDrawLine(renderer, camStart.x, camStart.y, endPoint.x, endPoint.y);
}

