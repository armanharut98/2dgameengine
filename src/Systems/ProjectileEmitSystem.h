#ifndef PROJECTILEEMITSYSTEM_H
#define PROJECTILEEMITSYSTEM_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"

class ProjectileEmitSystem : public System
{
public:
    ProjectileEmitSystem()
    {
        RequireComponent<ProjectileEmitterComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update(std::unique_ptr<Registry> &registry)
    {
        for (auto entity : GetSystemEntities())
        {
            auto &projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
            const auto &transform = entity.GetComponent<TransformComponent>();

            // Check if it is time to re-emit a new projectile
            if (SDL_GetTicks() - projectileEmitter.lastEmissionTime >= projectileEmitter.repeatFrequency)
            {
                glm::vec2 projectilePosition = transform.position;
                if (entity.HasComponent<SpriteComponent>())
                {
                    auto sprite = entity.GetComponent<SpriteComponent>();
                    projectilePosition.x += sprite.width * transform.scale.x / 2;
                    projectilePosition.y += sprite.height * transform.scale.y / 2;
                }

                // Add a new projectile entity to the registry
                Entity projectile = registry->CreateEntity();
                projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0);
                projectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
                projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
                projectile.AddComponent<BoxColliderComponent>(4, 4);
                projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);

                // Update the projectile emitter component last execution to the current milliseconds
                projectileEmitter.lastEmissionTime = SDL_GetTicks();
            }
        }
    }
};

#endif
