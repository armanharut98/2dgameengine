#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <set>
#include <deque>
#include <algorithm>
#include <memory>
#include <typeindex>
#include "../Logger/Logger.h"

const unsigned int MAX_COMPONENTS = 32;
////////////////////////////////////////////////////////////////////////////////////////
// SIGNATURE
////////////////////////////////////////////////////////////////////////////////////////
// We use a bitset (1s and 0s) to keep track of which components an entity has,
// and also helps keep track of which entities a given system is interested in.
////////////////////////////////////////////////////////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;

////////////////////////////////////////////////////////////////////////////////////////
// ENTITY
////////////////////////////////////////////////////////////////////////////////////////

class Entity
{
private:
    int id;

public:
    Entity(int id) : id(id) {};
    Entity(const Entity &entity) = default;
    int GetId() const;

    void Kill();

    Entity &operator=(const Entity &other) = default;
    bool operator==(const Entity &other) const;
    bool operator!=(const Entity &other) const;
    bool operator<(const Entity &other) const;
    bool operator>(const Entity &other) const;

    template <typename TComponent, typename... TArgs>
    void AddComponent(TArgs &&...args);
    template <typename TComponent>
    void RemoveComponent();
    template <typename TComponent>
    bool HasComponent() const;
    template <typename TComponent>
    TComponent &GetComponent() const;

    // Hold a pointer to the entity's owner registry
    class Registry *registry;
};

template <typename TComponent, typename... TArgs>
void Entity::AddComponent(TArgs &&...args)
{
    registry->template AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}
template <typename TComponent>
void Entity::RemoveComponent()
{
    registry->template RemoveComponent<TComponent>(*this);
}
template <typename TComponent>
bool Entity::HasComponent() const
{
    return registry->template HasComponent<TComponent>(*this);
}
template <typename TComponent>
TComponent &Entity::GetComponent() const
{
    return registry->template GetComponent<TComponent>(*this);
}

////////////////////////////////////////////////////////////////////////////////////////
// COMPONENT
////////////////////////////////////////////////////////////////////////////////////////

struct IComponent
{
protected:
    static int nextId;
};

// Used to assign a unique ID to a component type
template <typename TComponent>
class Component : public IComponent
{
public:
    // Returns the unique ID of Component<T>
    static int GetId()
    {
        // A static local variable is initialized only once and retains its value between function calls
        static auto id = nextId++;
        return id;
    }
};

////////////////////////////////////////////////////////////////////////////////////////
// SYSTEM
////////////////////////////////////////////////////////////////////////////////////////
// The system processes entities that contain a specific signature
////////////////////////////////////////////////////////////////////////////////////////

class System
{
private:
    Signature componentSignature;
    std::vector<Entity> entities;

protected:
    template <typename TComponent>
    void RequireComponent();

public:
    System() = default;
    ~System() = default;
    void AddEntity(Entity entity);
    const std::vector<Entity> &GetSystemEntities() const;
    void RemoveEntity(Entity entity);
    template <typename TComponent>
    void AddComponent(Component<TComponent> component);
    template <typename TComponent>
    void RemoveComponent(Component<TComponent> component);
    const Signature &GetComponentSignature() const;
};

template <typename TComponent>
void System::AddComponent(Component<TComponent> component)
{
    componentSignature.set(component.GetId());
}

template <typename TComponent>
void System::RemoveComponent(Component<TComponent> component)
{
    componentSignature.set(component.GetId(), false);
}

template <typename TComponent>
void System::RequireComponent()
{
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

////////////////////////////////////////////////////////////////////////////////////////
// POOL
////////////////////////////////////////////////////////////////////////////////////////
// A pool is just a vector (continguous data) of objects of type TComponent
////////////////////////////////////////////////////////////////////////////////////////

class IPool
{
public:
    virtual ~IPool() {}
};

template <typename TComponent>
class Pool : public IPool
{
private:
    std::vector<TComponent> data;

public:
    Pool(int size)
    {
        data.resize(size);
    }

    virtual ~Pool() = default;

    bool isEmpty() const
    {
        return data.empty();
    }

    int GetSize() const
    {
        return data.size();
    }

    void Resize(int n)
    {
        data.resize(n);
    }

    void Clear()
    {
        data.clear();
    }

    void Add(TComponent object)
    {
        data.push_back(object);
    }

    void Set(int index, TComponent object)
    {
        data[index] = object;
    }

    TComponent &Get(int index)
    {
        return static_cast<TComponent &>(data[index]);
    }

    TComponent &operator[](int index)
    {
        return data[index];
    }

    const TComponent &operator[](int index) const
    {
        return data[index];
    }
};

////////////////////////////////////////////////////////////////////////////////////////
// REGISTRY
////////////////////////////////////////////////////////////////////////////////////////
// The registry manages the creation and destruction of entities,
// add systems, and components
////////////////////////////////////////////////////////////////////////////////////////

class Registry
{
private:
    int nextEntityId = 0;
    std::set<Entity> entitiesToBeAdded;  // Entities awaiting creation in the next Registry.Update()
    std::set<Entity> entitiesToBeKilled; // Entities awaiting destruction in the next Registry.Update()
    std::deque<int> freeIDs;             // List of free entity IDs that were previously removed
    // Vector of component pools, each pool contains all the data for a certain component type
    // [Vector index = component type id]
    // [Pool index = entity id]
    std::vector<std::shared_ptr<IPool>> componentPools;
    // Vector of component signatures.
    // The signature lets us know which components are turned on for an entity
    // [Vector index = entity id]
    std::vector<Signature> entityComponentSignatures;
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

public:
    Registry() { Logger::Log("Registry constructor called"); };
    ~Registry() { Logger::Log("Registry destructor called"); };

    // Entity management
    Entity CreateEntity();
    void KillEntity(Entity Entity);
    void AddEntityToSystems(Entity entity);
    void RemoveEntityFromSystems(Entity entity);

    // Component management
    template <typename TComponent, typename... TArgs>
    void AddComponent(Entity entity, TArgs &&...args);
    template <typename TComponent>
    void RemoveComponent(Entity entity);
    template <typename TComponent>
    bool HasComponent(Entity entity) const;
    template <typename TComponent>
    TComponent &GetComponent(Entity entity) const;

    // System management
    template <typename TSystem, typename... TArgs>
    void AddSystem(TArgs &&...args);
    template <typename TSystem>
    void RemoveSystem();
    template <typename TSystem>
    bool HasSystem() const;
    template <typename TSystem>
    TSystem &GetSystem() const;

    void Update();
};

template <typename TComponent, typename... TArgs>
void Registry::AddComponent(Entity entity, TArgs &&...args)
{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    // If the component id is greater than the current size of the componentPools, then resize the vector
    if (componentId >= static_cast<int>(componentPools.size()))
    {
        componentPools.resize(componentId + 1, nullptr);
    }

    // If we still don't have a pool for that component type
    if (!componentPools[componentId])
    {
        componentPools[componentId] = std::make_shared<Pool<TComponent>>(10);
    }

    // Get the pool of component values for that component type
    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

    // if the entity id is greater than the current size of the component pool, then resize the pool
    if (entityId >= componentPool->GetSize())
    {
        componentPool->Resize(entityId + 1);
    }

    // Create a new Component object of the type TComponent, and forward the various parameters to the constructor
    TComponent component(std::forward<TArgs>(args)...);

    // Add the new component to the component pool list, using the entity id as index
    componentPool->Set(entityId, component);

    // Finally change the component signature of the entity and set the component id on the bitset to 1
    entityComponentSignatures[entityId].set(componentId);

    Logger::Log("Component id = ", componentId, " was added to entity id = ", entityId);
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity)
{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    entityComponentSignatures[entityId].set(componentId, false);

    Logger::Log("Component id = ", componentId, " was removed from entity id = ", entityId);
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity) const
{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    return entityComponentSignatures[entityId].test(componentId);
}

template <typename TComponent>
TComponent &Registry::GetComponent(Entity entity) const
{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    return componentPool->Get(entityId);
}

template <typename TSystem, typename... TArgs>
void Registry::AddSystem(TArgs &&...args)
{
    std::shared_ptr<TSystem> system = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), system));
}

template <typename TSystem>
void Registry::RemoveSystem()
{
    auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(system);
}

template <typename TSystem>
bool Registry::HasSystem() const
{
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem &Registry::GetSystem() const
{
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
}

#endif
