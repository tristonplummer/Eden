#include <shaiya/game/model/actor/npc/Npc.hpp>

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
    Actor::setPosition(position.translate(0, -0.88, 0));
}