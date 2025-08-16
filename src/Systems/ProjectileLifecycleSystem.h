#ifndef PROJECTILELIFECYCLESYSTEM_H
#define PROJECTILELIFECYCLESYSTEM_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"

class ProjectileLifecycleSystem : public System
{
public:
    ProjectileLifecycleSystem()
    {
        RequireComponent<ProjectileComponent>();
    }

    void Update()
    {
        for (auto entity : GetSystemEntities())
        {

            const auto &projectile = entity.GetComponent<ProjectileComponent>();
            if (SDL_GetTicks() - projectile.startTime >= projectile.duration)
            {
                entity.Kill();
            }
        }
    }
};

#endif
