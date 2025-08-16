#ifndef CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../Game/Game.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraFollowComponent.h"

class CameraMovementSystem : public System
{
public:
    CameraMovementSystem()
    {
        RequireComponent<CameraFollowComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update(SDL_Rect &camera)
    {
        for (auto entity : GetSystemEntities())
        {
            auto transform = entity.GetComponent<TransformComponent>();

            SDL_Point entityPos = {transform.position.x,
                                   transform.position.y};

            SDL_Point windowCenter = {Game::windowWidth / 2, Game::windowHeight / 2};

            if ((entityPos.x > windowCenter.x) && (entityPos.x + windowCenter.x < Game::mapWidth))
            {
                camera.x = entityPos.x - windowCenter.x;
            }
            if ((entityPos.y > windowCenter.y) && (entityPos.y + windowCenter.y < Game::mapHeight))
            {
                camera.y = entityPos.y - windowCenter.y;
            }
        }
    }
};

#endif
