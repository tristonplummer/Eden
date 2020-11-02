#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/model/Entity.hpp>
#include <shaiya/game/model/actor/StatSet.hpp>
#include <shaiya/game/model/actor/combat/CombatBuilder.hpp>
#include <shaiya/game/model/item/container/EquipmentContainer.hpp>
#include <shaiya/game/model/item/container/InventoryContainer.hpp>

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
         * Processes the tick for this entity.
         */
        void tick() override;

        /**
         * Activates this actor.
         */
        void activate() override;

        /**
         * Sets the position of this entity.
         * @param position  The position.
         */
        void setPosition(Position position) override;

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
         * Sets the level of the actor.
         * @param level The level value.
         */
        void setLevel(uint16_t level);

        /**
         * Synchronises this actor's stats, by calculating the bonuses from their
         * active buffs and worn equipment.
         */
        void syncStats();

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

        /**
         * Gets the equipment for this character.
         * @return  The equipment.
         */
        [[nodiscard]] EquipmentContainer& equipment()
        {
            return equipment_;
        }

        /**
         * Gets the equipment for this character.
         * @return  The equipment.
         */
        [[nodiscard]] const EquipmentContainer& equipment() const
        {
            return equipment_;
        }

        /**
         * Gets the inventory for this character.
         * @return  The inventory.
         */
        [[nodiscard]] InventoryContainer& inventory()
        {
            return inventory_;
        }

        /**
         * Gets the combat builder of the actor.
         * @return  The combat builder.
         */
        [[nodiscard]] CombatBuilder& combat()
        {
            return combat_;
        }

        /**
         * Gets the level of this actor.
         * @return  The level of the actor.
         */
        [[nodiscard]] uint16_t level() const
        {
            return level_;
        }

        /**
         * If this actor is running.
         * @return  If the movement is running.
         */
        [[nodiscard]] bool running() const
        {
            return running_;
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
         * The level of this actor.
         */
        uint16_t level_{ 0 };

        /**
         * If this actor is running.
         */
        bool running_{ false };

        /**
         * The faction of this actor.
         */
        shaiya::ShaiyaFaction faction_{ ShaiyaFaction::Neither };

        /**
         * The class of this actor.
         */
        shaiya::ShaiyaClass class_{ ShaiyaClass::Fighter };

        /**
         * The worn equipment of the actor.
         */
        EquipmentContainer equipment_;

        /**
         * The inventory of the actor.
         */
        InventoryContainer inventory_;

        /**
         * The combat builder of the actor.
         */
        CombatBuilder combat_;
    };
}