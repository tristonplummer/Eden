#pragma once
#include <shaiya/game/model/item/container/event/ContainerEventListener.hpp>

namespace shaiya::game
{
    /**
     * A specialised container event listener, that listens for events on a player's equipment.
     */
    class EquipmentEventListener: public ContainerEventListener
    {
    public:
        /**
         * Initialises this event listener for a character.
         * @param player The player that is listening to events.
         */
        explicit EquipmentEventListener(Player& player);

        /**
         * Gets executed when an item is added to a container.
         * @param container The container instance.
         * @param item      The item that was added.
         * @param slot      The slot of the new item.
         */
        void itemAdded(const ItemContainer& container, const std::shared_ptr<Item>& item, size_t slot) override;

        /**
         * Gets executed when an item is removed from a container.
         * @param container The container instance.
         * @param item      The item that was removed.
         * @param slot      The slot the item was removed from.
         */
        void itemRemoved(const ItemContainer& container, const std::shared_ptr<Item>& item, size_t slot) override;

        /**
         * Gets executed when an item is transferred between containers.
         * @param source        The source container.
         * @param dest          The destination container
         * @param sourceSlot    The source slot
         * @param destSlot      The destination slot.
         */
        void itemTransferred(const ItemContainer& source, const ItemContainer& dest, size_t sourceSlot,
                             size_t destSlot) override;

        /**
         * Synchronises the container with this listener.
         * @param container The container that was updated.
         */
        void sync(const ItemContainer& container) override;

    private:
        /**
         * The player that is listening for these updates.
         */
        Player& player_;
    };
}