#include "./ECS.h"

////////////////////////////////////////////////////////////////////////////////////////
// ENTITY
////////////////////////////////////////////////////////////////////////////////////////

int Entity::GetId() const
{
    return id;
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
    int entityId = nextEntityId++;
    Entity entity(entityId);
    entity.registry = this;
    entitiesToBeAdded.insert(entity);
    if (entityId >= static_cast<int>(entityComponentSignatures.size()))
    {
        entityComponentSignatures.resize(entityId + 1);
    }
    Logger::Log("Entity created with id: ", entityId);
    return entity;
}

void Registry::AddEntityToSystem(Entity entity)
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

void Registry::RemoveEntityFromSystem(Entity entity)
{
}

// Here is where we actually insert/delete the entities that are waiting to be added/removed.
// We do this because we don't want to confuse our Systems by adding/removing entities in the middle
// of the frame logic. Therefore, we wait until the end of the frame to update and perform the
// creation and deletion of entities.
void Registry::Update()
{
    for (auto entity : entitiesToBeAdded)
    {
        AddEntityToSystem(entity);
    }
    entitiesToBeAdded.clear();

    for (auto entity : entitiesToBeKilled)
    {
        RemoveEntityFromSystem(entity);
    }
    entitiesToBeKilled.clear();
}
