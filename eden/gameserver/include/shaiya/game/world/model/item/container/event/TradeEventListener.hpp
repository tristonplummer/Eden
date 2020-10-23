#pragma once
#include <shaiya/game/world/model/item/container/event/ContainerEventListener.hpp>

namespace shaiya::game
{
    /**
     * A specialised container event listener, that listens for events on a trade request container.
     */
    class TradeEventListener: public ContainerEventListener
    {
    public:
        /**
         * Initialises this event listener for a character.
         * @param player    The player who owns this container
         * @param partner   The trading partner
         */
        TradeEventListener(std::shared_ptr<Character> player, std::shared_ptr<Character> partner);

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
         * The player who owns this trade container.
         */
        std::shared_ptr<Character> player_;

        /**
         * The trading contaienr
         */
        std::shared_ptr<Character> partner_;
    };
}