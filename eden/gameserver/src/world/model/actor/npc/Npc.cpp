#include <shaiya/game/world/model/actor/npc/Npc.hpp>

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