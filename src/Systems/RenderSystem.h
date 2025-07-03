#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <SDL2/SDL.h>
#include <algorithm>

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
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

    void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore)
    {
        // Sort all the entities of our system by the z-index
        std::vector<Entity> entities = GetSystemEntities();
        std::sort(entities.begin(), entities.end(), [](const Entity &a, const Entity &b)
                  { return a.GetComponent<SpriteComponent>().zIndex < b.GetComponent<SpriteComponent>().zIndex; });

        // Loop all entities that the system is interested in
        for (auto entity : entities)
        {
            const auto transform = entity.GetComponent<TransformComponent>();
            const auto sprite = entity.GetComponent<SpriteComponent>();

            // Set the source rectangle of our original sprite texture
            SDL_Rect srcRect = sprite.srcRect;
            // Set the destination rectangle with the x and y position to be rendered in our SDL window
            SDL_Rect dstRect = {static_cast<int>(transform.position.x),
                                static_cast<int>(transform.position.y),
                                static_cast<int>(sprite.width * transform.scale.x),
                                static_cast<int>(sprite.height * transform.scale.y)};

            SDL_RenderCopyEx(renderer, assetStore->GetTexture(sprite.assetId), &srcRect, &dstRect, transform.rotation, NULL, SDL_FLIP_NONE);
        }
    }
};

#endif
