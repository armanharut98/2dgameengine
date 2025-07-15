#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"

#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"

class CollisionSystem : public System
{
public:
    CollisionSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }

    void Update()
    {
        auto entities = GetSystemEntities();
        // Loop all the entites that the system is interested in
        for (auto a = entities.begin(); a != entities.end(); a++)
        {
            // Loop all the entities that still need to be checked (to the right of a)
            for (auto b = a + 1; b != entities.end(); b++)
            {
                // Perform AABB collision check between entites a and b
                bool collisionHappened = CheckAABBCollision(*a, *b);
                if (collisionHappened)
                {
                    Logger::Log("Entities ", a->GetId(), " and ", b->GetId(), " have collided!");
                }
            }
        }
    }

private:
    bool CheckAABBCollision(Entity a, Entity b)
    {
        auto transformA = a.GetComponent<TransformComponent>();
        auto colliderA = a.GetComponent<BoxColliderComponent>();

        auto aX = transformA.position.x + colliderA.offset.x;
        auto aY = transformA.position.y + colliderA.offset.y;
        auto aW = colliderA.width;
        auto aH = colliderA.height;

        auto transformB = b.GetComponent<TransformComponent>();
        auto colliderB = b.GetComponent<BoxColliderComponent>();

        auto bX = transformB.position.x + colliderB.offset.x;
        auto bY = transformB.position.y + colliderB.offset.y;
        auto bW = colliderB.width;
        auto bH = colliderB.height;

        return (aX < bX + bW &&
                aX + aW > bX &&
                aY < bY + bH &&
                aY + aH > bY);
    }

    /* First attempt
        ((currentTransform.position.x < otherTransform.position.x + otherBoxCollider.width) && (currentTransform.position.y + currentBoxCollider.height > otherTransform.position.y)) &&
        ((currentTransform.position.x < otherTransform.position.x + otherBoxCollider.width) && (currentTransform.position.y < otherTransform.position.y + otherBoxCollider.height)) &&
        ((currentTransform.position.x + currentBoxCollider.width > otherTransform.position.x) && (currentTransform.position.y < otherTransform.position.y + otherBoxCollider.height)) &&
        ((currentTransform.position.x + currentBoxCollider.width > otherTransform.position.x) && (currentTransform.position.y + currentBoxCollider.height > otherTransform.position.y))
    */
};

#endif
