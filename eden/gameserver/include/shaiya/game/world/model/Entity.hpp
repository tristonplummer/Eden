#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/world/model/Attribute.hpp>
#include <shaiya/game/world/model/EntityType.hpp>
#include <shaiya/game/world/model/Position.hpp>
#include <shaiya/game/world/model/UpdateFlag.hpp>

#include <any>
#include <map>
#include <memory>

namespace shaiya::game
{
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
        virtual void activate();

        /**
         * Marks this entity as inactive.
         */
        void deactivate();

        /**
         * Flags this entity for an update of a specific type.
         * @param mask  The update type.
         */
        void flagUpdate(UpdateFlag mask);

        /**
         * Resets the update flags.
         */
        void resetUpdateFlags();

        /**
         * Sets the direction this entity is facing.
         * @param direction The direction to face.
         */
        void setDirection(uint16_t direction);

        /**
         * Sets the motion value of this entity.
         * @param motion    The motion value.
         */
        void setMotion(uint8_t motion);

        /**
         * Checks if this entity can be observed by another.
         * @param other The entity trying to observe this entity.
         * @return      If the other entity can observe us.
         */
        virtual bool observable(Entity& other);

        /**
         * Sets the id for this entity.
         * @param id    The new id.
         */
        void setId(size_t id);

        /**
         * Checks if this entity is flagged for an update of a specific type.
         * @param mask  The update type.
         * @return      If the entity is flagged for this update.
         */
        bool hasUpdateFlag(UpdateFlag mask) const
        {
            return updateMask_ & static_cast<uint32_t>(mask);
        }

        /**
         * Checks if this entity is active.
         * @return  If the entity is active.
         */
        bool active() const
        {
            return active_;
        }

        /**
         * Gets the direction that the entity is facing.
         * @return  The direction.
         */
        uint16_t direction() const
        {
            return direction_;
        }

        /**
         * Gets the motion value of the entity.
         * @return  The motion.
         */
        uint8_t motion() const
        {
            return motion_;
        }

        /**
         * Sets an attribute.
         * @param attribute The attribute key.
         * @param value     The value to set.
         */
        void setAttribute(Attribute attribute, std::any value);

        /**
         * Clears an attribute.
         * @param attribute The attribute key.
         */
        void clearAttribute(Attribute attribute);

        /**
         * Gets an attribute.
         * @tparam T            The attribute type.
         * @param key           The attribute key.
         * @param defaultValue  The default value.
         * @return              The attribute value.
         */
        template<typename T>
        T getAttribute(Attribute key, T defaultValue = {}) const
        {
            if (attributes_.contains(key))
            {
                return std::any_cast<T>(attributes_.at(key));
            }
            return defaultValue;
        }

        /**
         * Get the position of this entity.
         * @return  The position
         */
        [[nodiscard]] Position& position()
        {
            return position_;
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

        /**
         * Gets the current map of this entity.
         * @return  The current map.
         */
        [[nodiscard]] std::shared_ptr<Map> map() const;

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
         * The position of this entity in the world.
         */
        Position position_;

        /**
         * If this entity is active.
         */
        bool active_{ false };

        /**
         * The update mask of this entity.
         */
        uint32_t updateMask_{ 0 };

        /**
         * The motion value of this entity.
         */
        uint8_t motion_{ 0 };

        /**
         * The direction this entity is facing.
         */
        uint16_t direction_{ 0 };

        /**
         * The attributes of this entity. An attribute can be any temporary, not serialized state.
         */
        std::map<Attribute, std::any> attributes_;
    };
}