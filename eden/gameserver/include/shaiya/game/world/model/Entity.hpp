#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/game/world/model/Position.hpp>

namespace shaiya::game
{
    /**
     * An entity can be thought of as any object in the game world that exist as a point in the world space. Items that are
     * dropped on the ground, characters, mobs and invisible points are all examples of entities.
     */
    class Entity
    {
    public:
        /**
         * Initialises this entity.
         */
        virtual void init();

        /**
         * Get the position of this entity.
         * @return  The position
         */
        [[nodiscard]] Position& position()
        {
            return position_;
        }

        /**
         * Gets the id of this entity.
         * @return  The id.
         */
        [[nodiscard]] size_t id() const
        {
            return id_;
        }

    protected:
        /**
         * The id of this entity..
         */
        size_t id_{ 0 };

        /**
         * The position of this entity in the world.
         */
        Position position_;
    };
}