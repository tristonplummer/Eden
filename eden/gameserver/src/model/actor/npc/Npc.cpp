#include <shaiya/game/model/actor/npc/Npc.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

using namespace shaiya::game;

/**
 * Initialises an NPC with a specified definition.
 * @param def       The npc definition.
 * @param world     The game world instance.
 */
Npc::Npc(const NpcDefinition& def, GameWorldService& world): Actor(world), def_(def)
{
    type_ = EntityType::Npc;
}

/**
 * Sets the position of this entity.
 * @param position  The position.
 */
void Npc::setPosition(Position position)
{
    auto map = world().maps().forId(position.map());

    if (map)
    {
        float y = map->heightmap().y(position.x(), position.z());
        if (y > position.y())
            position = Position(position.map(), position.x(), y, position.z());
    }

    Actor::setPosition(position);
}