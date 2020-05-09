#pragma once

#include "Common/Entity/Player.h"
#include "Common/Command.h"

#include "Net/Packet.h"

class EntitySpawnCommand : public EntityCommand
{
public:
    unsigned id;
    sh::Transform transform;

    virtual void Execute(std::vector<std::unique_ptr<Entity>>& entities) override final
    {
        auto entity = std::make_unique<Player>();
        entity->transform = transform;
        entity->id = id;

        entities.push_back(std::move(entity)); 
    }
    virtual void Undo(std::vector<std::unique_ptr<Entity>>& entities) override final { SH_ASSERT(false, "Not yet implemented"); }

    template <typename T>
    void serialize(T& archive)
    {
        archive(cereal::base_class<EntityCommand>(this));
        archive(id);
        archive(transform);
    }
};
CEREAL_REGISTER_TYPE(EntitySpawnCommand)

class EntityRemoveCommand : public EntityCommand
{
public:
    unsigned entityID;

    virtual void Execute(std::vector<std::unique_ptr<Entity>>& entities) override final { SH_ASSERT(false, "Not yet implemented"); }
    virtual void Undo(std::vector<std::unique_ptr<Entity>>& entities) override final { SH_ASSERT(false, "Not yet implemented"); }

    template <typename T>
    void serialize(T& archive)
    {
        archive(cereal::base_class<EntityCommand>(this));
        archive(entityID);
    }
};
CEREAL_REGISTER_TYPE(EntityRemoveCommand)