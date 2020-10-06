#pragma once
#include <shaiya/game/world/model/item/container/ItemContainer.hpp>

namespace shaiya::game
{
    class Character;

    /**
     * A specialised container that acts as a character's inventory.
     */
    class InventoryContainer: public ItemContainer
    {
    public:
        /**
         * Initialises this inventory. container.
         * @param character The character that owns this inventory.
         */
        explicit InventoryContainer(Character& character);

    private:
        /**
         * The character that owns this inventory container.
         */
        Character& character_;
    };
}