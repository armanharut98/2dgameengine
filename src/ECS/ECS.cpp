#include "./ECS.h"

////////////////////////////////////////////////////////////////////////////////////////
// ENTITY
////////////////////////////////////////////////////////////////////////////////////////

int Entity::GetId() const
{
    return id;
}

void Entity::Kill()
{
    registry->KillEntity(*this);
}

bool Entity::operator==(const Entity &other) const
{
    return id == other.GetId();
}

bool Entity::operator!=(const Entity &other) const
{
    return id != other.GetId();
}

bool Entity::operator>(const Entity &other) const
{
    return id > other.GetId();
}

bool Entity::operator<(const Entity &other) const
{
    return id > other.GetId();
}

////////////////////////////////////////////////////////////////////////////////////////
// COMPONENT
////////////////////////////////////////////////////////////////////////////////////////

int IComponent::nextId = 0;

////////////////////////////////////////////////////////////////////////////////////////
// SYSTEM
////////////////////////////////////////////////////////////////////////////////////////
// The system processes entities that contain a specific signature
////////////////////////////////////////////////////////////////////////////////////////

const std::vector<Entity> &System::GetSystemEntities() const
{
    return entities;
}

void System::AddEntity(Entity entity)
{
    entities.push_back(entity);
}

void System::RemoveEntity(Entity entity)
{
    entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other)
                                  { return entity == other; }),
                   entities.end());
}

const Signature &System::GetComponentSignature() const
{
    return componentSignature;
}

////////////////////////////////////////////////////////////////////////////////////////
// REGISTRY
////////////////////////////////////////////////////////////////////////////////////////
// The registry manages the creation and destruction of entities,
// add systems, and components
////////////////////////////////////////////////////////////////////////////////////////

Entity Registry::CreateEntity()
{
    int entityId;
    if (freeIDs.empty())
    {
        entityId = nextEntityId++;
        if (entityId >= static_cast<int>(entityComponentSignatures.size()))
        {
            entityComponentSignatures.resize(entityId + 1);
        }
    }
    else
    {
        entityId = freeIDs.front();
        freeIDs.pop_front();
    }
    Entity entity(entityId);
    entity.registry = this;
    entitiesToBeAdded.insert(entity);
    Logger::Log("Entity created with id: ", entityId);
    return entity;
}

void Registry::KillEntity(Entity entity)
{
    entitiesToBeKilled.insert(entity);
    Logger::Log("Entity killed with id: ", entity.GetId());
}

void Registry::AddEntityToSystems(Entity entity)
{
    const auto entityComponentSignature = entityComponentSignatures[entity.GetId()];
    for (const auto &system : systems)
    {
        const auto &systemComponentSignature = system.second->GetComponentSignature();
        if ((entityComponentSignature & systemComponentSignature) == systemComponentSignature)
        {
            system.second->AddEntity(entity);
        }
    }
}

void Registry::RemoveEntityFromSystems(Entity entity)
{
    for (const auto &system : systems)
    {
        system.second->RemoveEntity(entity);
    }
}

// Here is where we actually insert/delete the entities that are waiting to be added/removed.
// We do this because we don't want to confuse our Systems by adding/removing entities in the middle
// of the frame logic. Therefore, we wait until the end of the frame to update and perform the
// creation and deletion of entities.
void Registry::Update()
{
    for (auto entity : entitiesToBeAdded)
    {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();

    for (auto entity : entitiesToBeKilled)
    {
        RemoveEntityFromSystems(entity);
        entityComponentSignatures[entity.GetId()].reset();
        freeIDs.push_back(entity.GetId());
    }
    entitiesToBeKilled.clear();
}
