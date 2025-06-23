//
// Created by shawn on 5/24/2025.
//

#include "Raycaster.h"
#include <float.h>
#include <bits/stl_algo.h>

#include "../Camera.h"
#include "../../input.h"

bool Raycaster::lineIntersectsRect(const Vector2& rayStart, const Vector2& rayEnd, const Vector2& position, const Collider* collider, Vector2* intersection) {

      Vector2 calcRayEnd = rayEnd;
      calcRayEnd.x = static_cast<int>(calcRayEnd.x);
      calcRayEnd.y = static_cast<int>(calcRayEnd.y);
      //some base case statement
      if(rayStart.x == calcRayEnd.x && rayStart.y == calcRayEnd.y)
            return false;

      const Vector2 camPosition = Camera::mainCamera == nullptr ? Vector2{ 0, 0 } : Camera::mainCamera->transform->getPosition();

      const Vector2 b = position - camPosition;

      if(rayStart.x == b.x && rayStart.y == b.y)
            return false; //calling object

      //a to c
      const Vector2 ac(calcRayEnd.x - rayStart.x, calcRayEnd.y - rayStart.y);

      //a to b
      const Vector2 ab(b.x - rayStart.x, b.y - rayStart.y);

      //calculate the projection scalar for this object
      double projectionScalar = (ab.x * ac.x + ab.y * ac.y) / (pow(ac.x, 2.0) + pow(ac.y, 2.0));
      projectionScalar = std::max(0.0, std::min(1.0, projectionScalar));

      //find the closest point that this may interesect, allows for thresholds
      intersection->x = rayStart.x + projectionScalar * (calcRayEnd.x - rayStart.x);
      intersection->y = rayStart.y + projectionScalar * (calcRayEnd.y - rayStart.y);

      //grab the collider size
      const Vector2 colliderSize = collider->getSize();

      //true if these both intersect
      return (intersection->x >= b.x - colliderSize.x) && (intersection->x <= b.x + colliderSize.x) && (intersection->y >= b.y - colliderSize.y) && (intersection->y <= b.y + colliderSize.y);
}

Vector2 Raycaster::createEndPoint(const Ray &ray) {

      const Vector2 normalized = ray.angle.normalize();

      Vector2 ret(
          ray.position.x + normalized.x * ray.distance,
          ray.position.y + normalized.y * ray.distance
      );

      return ret;
}

bool Raycaster::castUI(RayInfo *rayInformation) {

      const Ray mouseRay(input::getMousePosition(), 0, 10);

      const Vector2& rayEndPoint = createEndPoint(mouseRay);

      auto closestDistance = DBL_MAX;
      rayInformation->castFrom = mouseRay.position;

      for(auto* ui : UiObject::getRegisteredUI()) {

            if(ui->collider == nullptr) {
                  //purely decorational.
                  continue;
            }

            Vector2 intersection;
            if(lineIntersectsRect(mouseRay.position, rayEndPoint, ui->position, ui->collider, &intersection)) {
                  const double objDistanceToRay = mouseRay.position.distance(ui->position);

                  if(objDistanceToRay < closestDistance) {
                        closestDistance = objDistanceToRay;
                        rayInformation->collider = ui->collider;
                        rayInformation->uiObjectHit = ui;
                        rayInformation->positionHit = intersection;
                  }
            }

      }

      return rayInformation->uiObjectHit != nullptr;
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
                  continue;


            const Vector2 objPosition = obj->transform->getPosition();

            Vector2 intersection;
            //it has hit something
            if(lineIntersectsRect(ray.position, rayEndPoint, obj->transform->getPosition(), obj->collider, &intersection)) {
                  const double objDistanceToRay = ray.position.distance(objPosition);

                  //assume if 0, it is the caller
                  if(objDistanceToRay < closestDistance) {
                        //select this as the clsoest item now
                        closestDistance = objDistanceToRay;
                        rayInformation->collider = obj->collider;
                        rayInformation->gameObjectHit = obj;
                        rayInformation->positionHit = intersection;
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
      const Vector2 camStart = ray.position - camPos;
      const Vector2 endPoint = isHit ? rInfo.positionHit - camPos : createEndPoint(ray) - camPos;

      //create a white line if it is not hit, green if it is
      const Uint8 r = isHit ? 0 : 255;
      const Uint8 b = isHit ? 0 : 255;

      SDL_SetRenderDrawColor(renderer, r, 255, b, 255);
      SDL_RenderDrawLine(renderer, camStart.x, camStart.y, endPoint.x, endPoint.y);
}

//Create a cast from the mouse
bool Raycaster::castFromMouse(RayInfo *rayInformation, const double &radius, SDL_Renderer *shouldRender) {

      Vector2 mouse = input::getMousePosition();

      if (Camera::mainCamera != nullptr) {
            mouse += Camera::mainCamera->transform->getPosition();
      }

      for (int i = 0; i < 4; i++) {
            const Ray ray(mouse, 90 * i, radius);

            if (cast(ray, rayInformation))
                  return true;

            if (shouldRender != nullptr) {
                  drawCast(ray, shouldRender, *rayInformation);
            }
      }

      return false;
}


