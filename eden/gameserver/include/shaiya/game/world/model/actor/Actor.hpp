#pragma once
#include <shaiya/game/world/model/Entity.hpp>
#include <shaiya/game/world/model/actor/StatSet.hpp>

namespace shaiya::game
{
    /**
     * An actor is any entity that has the ability to traverse space and interact with other entities, such as in combat or
     * communications. Players and mobs are good examples of actors.
     */
    class Actor: public Entity
    {
    public:
        /**
         * Initialises this actor.
         * @param world The world instance.
         */
        explicit Actor(GameWorldService& world);

        /**
         * Initialises this actor.
         */
        void init() override;

        /**
         * Gets the stats for this actor.
         * @return  The actor's stats.
         */
        [[nodiscard]] StatSet& stats()
        {
            return stats_;
        }

    protected:
        /**
         * The stats of the actor.
         */
        StatSet stats_;

        /**
         * The faction of this actor.
         */
        shaiya::ShaiyaFaction faction_{ ShaiyaFaction::Neither };
    };
}