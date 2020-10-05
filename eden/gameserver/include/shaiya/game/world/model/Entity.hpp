#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/game/world/model/EntityType.hpp>
#include <shaiya/game/world/model/Position.hpp>

#include <memory>

namespace shaiya::game
{
    class GameWorldService;  // Forward declaration of the game service.

    /**
     * An entity can be thought of as any object in the game world that exist as a point in the world space. Items that are
     * dropped on the ground, characters, mobs and invisible points are all examples of entities.
     */
    class Entity: public std::enable_shared_from_this<Entity>
    {
    public:
        /**
         * Initialise this entity.
         * @param world The game world.
         */
        explicit Entity(GameWorldService& world);

        /**
         * Initialises this entity.
         */
        virtual void init();

        /**
         * Sets the position of this entity.
         * @param position  The position.
         */
        void setPosition(Position position);

        /**
         * Marks this entity as active.
         */
        void activate();

        /**
         * Marks this entity as inactive.
         */
        void deactivate();

        /**
         * Checks if this entity is active.
         * @return  If the entity is active.
         */
        bool active() const
        {
            return active_;
        }

        /**
         * Get the position of this entity.
         * @return  The position
         */
        [[nodiscard]] const Position& position() const
        {
            return position_;
        }

        /**
         * Gets the game world service.
         * @return  The world service.
         */
        [[nodiscard]] GameWorldService& world() const
        {
            return world_;
        }

        /**
         * Gets the type of this entity.
         * @return  The type.
         */
        [[nodiscard]] EntityType type() const
        {
            return type_;
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
         * The id of this entity.
         */
        size_t id_{ 0 };

        /**
         * The type of this entity.
         */
        EntityType type_{ EntityType::Entity };

    private:
        /**
         * The game world instance.
         */
        GameWorldService& world_;

        /**
         * If this entity is active.
         */
        bool active_{ false };

        /**
         * The position of this entity in the world.
         */
        Position position_;
    };
}