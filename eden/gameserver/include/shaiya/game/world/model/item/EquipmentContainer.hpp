#pragma once
#include <shaiya/game/world/model/item/EquipmentSlot.hpp>
#include <shaiya/game/world/model/item/ItemContainer.hpp>

namespace shaiya::game
{
    class Character;

    /**
     * A specialised container that acts as a character's worn equipment.
     */
    class EquipmentContainer: public ItemContainer
    {
    public:
        /**
         * Initialises this equipment container.
         * @param character The character that owns this equipment.
         */
        explicit EquipmentContainer(Character& character);

        /**
         * Adds an item to this container.
         * @param item  The item to add.
         * @return      If the item was successfully added.
         */
        bool add(std::shared_ptr<Item> item) override;

        /**
         * Adds an item to this container at a specific slot.
         * @param item  The item to add.
         * @param slot  The requested slot.
         * @return      If the item was successfully added.
         */
        bool add(std::shared_ptr<Item> item, EquipmentSlot slot);

    private:
        /**
         * The character that owns this equipment container.
         */
        Character& character_;
    };
}