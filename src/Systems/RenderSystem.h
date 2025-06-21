#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"

class RenderSystem : public System
{
public:
    RenderSystem()
    {
        RequireComponent<SpriteComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update(SDL_Renderer *renderer)
    {
        for (auto entity : GetSystemEntities())
        {
            const auto transform = entity.GetComponent<TransformComponent>();
            const auto sprite = entity.GetComponent<SpriteComponent>();

            SDL_Rect objRect = {transform.position.x,
                                transform.position.y,
                                sprite.width,
                                sprite.height};

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &objRect);
        }
    }
};

#endif
