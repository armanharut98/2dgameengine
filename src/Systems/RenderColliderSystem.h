#ifndef RENDERCOLLIDER_H
#define RENDERCOLLIDER_H

#include "../Logger/Logger.h"
#include "../SDL2/SDL.h"

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

class RenderCollisionSystem : public System
{
public:
    RenderCollisionSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }

    void Update(SDL_Renderer *renderer, SDL_Rect &camera)
    {
        for (auto entity : GetSystemEntities())
        {
            auto &transform = entity.GetComponent<TransformComponent>();
            auto &boxCollider = entity.GetComponent<BoxColliderComponent>();

            SDL_Rect rect = {transform.position.x + boxCollider.offset.x - camera.x,
                             transform.position.y + boxCollider.offset.y - camera.y,
                             boxCollider.height * transform.scale.x,
                             boxCollider.width * transform.scale.y};

            if (boxCollider.isColliding)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }

            SDL_RenderDrawRect(renderer, &rect);
        }
    }
};

#endif
