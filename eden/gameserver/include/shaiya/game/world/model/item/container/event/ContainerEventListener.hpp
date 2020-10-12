#pragma once
#include <memory>

namespace shaiya::game
{
    class Item;
    class ItemContainer;

    /**
     * A utility class that listens for events performed by a container.
     */
    class ContainerEventListener
    {
    public:
        /**
         * Gets executed when an item is added to a container.
         * @param container The container instance.
         * @param item      The item that was added.
         * @param slot      The slot of the new item.
         */
        virtual void itemAdded(const ItemContainer& container, const std::shared_ptr<Item>& item, size_t slot) = 0;

        /**
         * Gets executed when an item is removed from a container.
         * @param container The container instance.
         * @param item      The item that was removed.
         * @param slot      The slot the item was removed from.
         */
        virtual void itemRemoved(const ItemContainer& container, const std::shared_ptr<Item>& item, size_t slot) = 0;

        /**
         * Gets executed when an item is transferred between containers.
         * @param source        The source container.
         * @param dest          The destination container
         * @param sourceSlot    The source slot
         * @param destSlot      The destination slot.
         */
        virtual void itemTransferred(const ItemContainer& source, const ItemContainer& dest, size_t sourceSlot,
                                     size_t destSlot)
        {
        }

        /**
         * Synchronises the container with this listener.
         * @param container The container that was updated.
         */
        virtual void sync(const ItemContainer& container) = 0;
    };
}