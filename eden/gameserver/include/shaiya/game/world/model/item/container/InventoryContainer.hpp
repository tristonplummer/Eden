#pragma once
#include <shaiya/game/world/model/item/container/ItemContainer.hpp>

namespace shaiya::game
{
    class Character;

    /**
     * A specialised container that acts as an actor's inventory.
     */
    class InventoryContainer: public ItemContainer
    {
    public:
        /**
         * Initialises this inventory container.
         */
        explicit InventoryContainer();
    };
}