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
         * Sets the name of this actor.
         * @param name  The name value.
         */
        void setName(const std::string& name);

        /**
         * Sets the class of the actor.
         * @param job   The class value.
         */
        void setJob(shaiya::ShaiyaClass job);

        /**
         * Gets the stats for this actor.
         * @return  The actor's stats.
         */
        [[nodiscard]] StatSet& stats()
        {
            return stats_;
        }

        /**
         * Gets the name of this actor.
         * @return  The actor's name.
         */
        [[nodiscard]] const std::string& name() const
        {
            return name_;
        }

        /**
         * Gets the class of this actor.
         * @return  The actor's class.
         */
        [[nodiscard]] shaiya::ShaiyaClass job() const
        {
            return class_;
        }

        /**
         * Gets the faction of this actor.
         * @return  The actor's faction.
         */
        [[nodiscard]] shaiya::ShaiyaFaction faction() const
        {
            return faction_;
        }

    protected:
        /**
         * The name of this actor.
         */
        std::string name_;

        /**
         * The stats of the actor.
         */
        StatSet stats_;

        /**
         * The faction of this actor.
         */
        shaiya::ShaiyaFaction faction_{ ShaiyaFaction::Neither };

        /**
         * The class of this actor.
         */
        shaiya::ShaiyaClass class_{ ShaiyaClass::Fighter };
    };
}