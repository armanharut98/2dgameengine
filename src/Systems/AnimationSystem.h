#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include <SDL2/SDL.h>

#include "../ECS/ECS.h"

#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"

class AnimationSystem : public System
{
public:
    AnimationSystem()
    {
        RequireComponent<AnimationComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update()
    {
        for (auto entity : GetSystemEntities())
        {
            auto &animation = entity.GetComponent<AnimationComponent>();
            auto &sprite = entity.GetComponent<SpriteComponent>();

            animation.currentFrame = ((SDL_GetTicks() - animation.startTime) / (1000 / animation.frameSpeedRate)) % animation.numFrames;
            sprite.srcRect.x = animation.currentFrame * sprite.width;
        }
    }
};

#endif
