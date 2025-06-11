#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <algorithm>

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
    int GetId() const;
    bool operator==(const Entity &other) const;
    bool operator!=(const Entity &other) const;
};

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
    void RemoveComponent(Component<TComponent> componentn);
    const Signature &GetComponentSignature() const;
};

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
// REGISTRY
////////////////////////////////////////////////////////////////////////////////////////

class Registry
{
};

#endif
