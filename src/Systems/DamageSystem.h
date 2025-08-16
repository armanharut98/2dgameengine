#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Components/BoxColliderComponent.h"

class DamageSystem : public System
{
private:
    void OnCollision(CollisionEvent &event)
    {
        /*
        event.a.Kill();
        event.b.Kill();
        */
        Logger::Log("Damage system received a CollisionEvent between entities ", event.a.GetId(), " and ", event.b.GetId());
    }

public:
    DamageSystem()
    {
        RequireComponent<BoxColliderComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus)
    {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
    }

    void Update()
    {
        // TODO
    }
};

#endif
