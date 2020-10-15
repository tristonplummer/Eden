#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/world/model/actor/Actor.hpp>
#include <shaiya/game/world/model/actor/npc/NpcDefinition.hpp>

namespace shaiya::game
{
    /**
     * Represents a non-player character. An NPC is an actor that can be interacted with, such as a merchant or a
     * quest-giver. NPCs cannot be attacked by other actors, but can attack actors (such as a Guard).
     */
    class Npc: public Actor
    {
    public:
        /**
         * Initialises an NPC with a specified definition.
         * @param def       The npc definition.
         * @param world     The game world instance.
         */
        Npc(const NpcDefinition& def, GameWorldService& world);

        /**
         * Gets the definition for this npc.
         * @return  The definition.
         */
        [[nodiscard]] const NpcDefinition& definition() const
        {
            return def_;
        }

    private:
        /**
         * The definition of this npc.
         */
        const NpcDefinition& def_;
    };
}