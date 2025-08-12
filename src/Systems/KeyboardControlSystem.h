#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/KeyboardControlledComponent.h"

class KeyboardControlSystem : public System
{
private:
    void OnKeyPressed(KeyPressedEvent &event)
    {
        for (auto entity : GetSystemEntities())
        {
            const auto &keyboardControl = entity.GetComponent<KeyboardControlledComponent>();
            auto &sprite = entity.GetComponent<SpriteComponent>();
            auto &rigidBody = entity.GetComponent<RigidBodyComponent>();

            switch (event.symbol)
            {
            case SDLK_UP:
                sprite.srcRect.y = sprite.height * 0;
                rigidBody.velocity = keyboardControl.upVelocity;
                break;
            case SDLK_RIGHT:
                sprite.srcRect.y = sprite.height * 1;
                rigidBody.velocity = keyboardControl.rightVelocity;
                break;
            case SDLK_DOWN:
                sprite.srcRect.y = sprite.height * 2;
                rigidBody.velocity = keyboardControl.downVelocity;
                break;
            case SDLK_LEFT:
                sprite.srcRect.y = sprite.height * 3;
                rigidBody.velocity = keyboardControl.leftVelocity;
                break;
            }
        }
    }

public:
    KeyboardControlSystem()
    {
        RequireComponent<KeyboardControlledComponent>();
        RequireComponent<SpriteComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus)
    {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
    }

    void Update()
    {
    }
};

#endif
