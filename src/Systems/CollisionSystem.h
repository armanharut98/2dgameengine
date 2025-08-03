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
            auto &transformA = a->GetComponent<TransformComponent>();
            auto &boxColliderA = a->GetComponent<BoxColliderComponent>();

            boxColliderA.isColliding = false;
            // Loop all the entities that still need to be checked (to the right of a)
            for (auto b = entities.begin(); b != entities.end(); b++)
            {
                if (a != b)
                {
                    auto &transformB = b->GetComponent<TransformComponent>();
                    auto &boxColliderB = b->GetComponent<BoxColliderComponent>();

                    // Perform AABB collision check between entites a and b
                    bool collisionHappened = CheckAABBCollision(transformA, boxColliderA, transformB, boxColliderB);
                    if (collisionHappened)
                    {
                        Logger::Log("Entities ", a->GetId(), " and ", b->GetId(), " have collided!");
                        if (!boxColliderA.isColliding)
                        {

                            boxColliderA.isColliding = true;
                        }
                    }
                }
            }
        }
    }

private:
    bool CheckAABBCollision(const TransformComponent &transformA, const BoxColliderComponent &boxColliderA, const TransformComponent &transformB, const BoxColliderComponent &boxColliderB)
    {
        auto aX = transformA.position.x + boxColliderA.offset.x;
        auto aY = transformA.position.y + boxColliderA.offset.y;
        auto aW = boxColliderA.width;
        auto aH = boxColliderA.height;

        auto bX = transformB.position.x + boxColliderB.offset.x;
        auto bY = transformB.position.y + boxColliderB.offset.y;
        auto bW = boxColliderB.width;
        auto bH = boxColliderB.height;

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
