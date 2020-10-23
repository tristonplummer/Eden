#pragma once
#include <shaiya/game/world/model/item/container/event/ContainerEventListener.hpp>

namespace shaiya::game
{
    /**
     * An event listener that listens while in a trade session.
     */
    class TradeEventListener: public ContainerEventListener
    {
    public:
        /**
         * Initialises the trade event listener.
         * @param character The character to listen for.
         * @param trade     The trade session.
         */
        TradeEventListener(std::shared_ptr<Player> character, TradeRequest& trade);

        /**
         * Gets executed when an item is moved from the inventory.
         * @param source        The source container.
         * @param dest          The destination container.
         * @param sourceSlot    The source slot.
         * @param destSlot      The destination slot.
         */
        void itemTransferred(const ItemContainer& source, const ItemContainer& dest, size_t sourceSlot,
                             size_t destSlot) override;

    private:
        /**
         * The character instance.
         */
        std::shared_ptr<Player> character_;

        /**
         * The trade session.
         */
        TradeRequest& trade_;
    };
}