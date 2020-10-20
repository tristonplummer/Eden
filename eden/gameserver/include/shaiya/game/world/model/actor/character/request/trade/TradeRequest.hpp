#pragma once
#include <shaiya/common/net/packet/game/CharacterTradeRequest.hpp>
#include <shaiya/game/world/model/actor/character/request/Request.hpp>

namespace shaiya::game
{
    /**
     * Represents a trading request between players.
     */
    class TradeRequest: public Request
    {
    public:
        /**
         * Initialises this request.
         * @param player    The player who made the request.
         * @param partner   The request partner.
         */
        TradeRequest(std::shared_ptr<Character> player, std::shared_ptr<Character> partner);

        /**
         * Opens this request.
         */
        void open() override;

        /**
         * Offers an amount of gold into the trade window.
         * @param offeredGold   The requested amount of gold.
         */
        void offerGold(size_t offeredGold);

        /**
         * Offers an item up for trade.
         * @param slot      The slot.
         * @param quantity  The quantity to offer.
         * @param destSlot  The destination slot in the trade window.
         * @return          If the item was added.
         */
        bool offerItem(size_t slot, size_t quantity, size_t destSlot);

        /**
         * Removes an item from the trade window.
         * @param slot  The slot.
         */
        void removeItem(size_t slot);

        /**
         * Accepts the trade.
         */
        void accept();

        /**
         * Cancels the trade.
         */
        void close() override;

        /**
         * Confirms the current trade window.
         */
        void confirm();

        /**
         * Unconfirms the current trade window.
         */
        void unconfirm();

    private:
        /**
         * Finalises the trade.
         */
        void finalise();

        /**
         * Notifies players about the state of the trade request (confirmation, accept)
         */
        void notifyState();

        /**
         * A container of items our player has offered to trade.
         */
        std::unique_ptr<ItemContainer> container_;

        /**
         * A copy of the player's inventory.
         */
        std::unique_ptr<InventoryContainer> inventory_;

        /**
         * The amount of gold our player has offered to trade.
         */
        size_t gold_{ 0 };

        /**
         * If our player has confirmed the current trade window.
         */
        bool confirmed_{ false };

        /**
         * If our player has accepted the current trade window. This is only available after both
         * participants have confirmed the trade. When both players accept, the trade is deemed completed.
         */
        bool accepted_{ false };

        /**
         * If the trade has been finalised.
         */
        bool finalised_{ false };
    };
}