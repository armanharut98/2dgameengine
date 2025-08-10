#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"

class KeyboardControlSystem : public System
{
private:
    void OnKeyPressed(KeyPressedEvent &event)
    {
        Logger::Log("Key pressed event emitted: ", SDL_GetKeyName(event.symbol));
    }

public:
    KeyboardControlSystem() {}

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus)
    {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
    }

    void Update() {}
};

#endif
