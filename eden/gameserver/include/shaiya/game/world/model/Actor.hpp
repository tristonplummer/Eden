#pragma once
#include <shaiya/game/world/model/Entity.hpp>

namespace shaiya::game
{
    /**
     * An actor is any entity that has the ability to traverse space and interact with other entities, such as in combat or
     * communications. Players and mobs are good examples of actors.
     */
    class Actor: public Entity
    {
    public:
    protected:
        /**
         * The faction of this actor.
         */
        shaiya::ShaiyaFaction faction_{ ShaiyaFaction::Neither };
    };
}