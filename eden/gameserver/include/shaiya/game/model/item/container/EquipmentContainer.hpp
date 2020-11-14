#pragma once
#include <shaiya/game/model/item/container/EquipmentSlot.hpp>
#include <shaiya/game/model/item/container/ItemContainer.hpp>

namespace shaiya::game
{
    /**
     * A specialised container that acts as a actor's worn equipment.
     */
    class EquipmentContainer: public ItemContainer
    {
    public:
        /**
         * Initialises this equipment container.
         */
        explicit EquipmentContainer();

        /**
         * Gets the item at a specific page and slot
         * @param page  The page
         * @param slot  The slot
         * @return      The item at the requested position
         */
        [[nodiscard]] std::shared_ptr<Item> at(EquipmentSlot slot) const;

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

        /**
         * Gets the vector of items that are visible by other characters.
         * @return  The visible items
         */
        [[nodiscard]] std::vector<std::shared_ptr<Item>> visibleItems() const;
    };
}