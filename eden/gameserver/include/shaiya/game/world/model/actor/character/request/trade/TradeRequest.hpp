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
         * Cancels the trade.
         */
        void cancel();

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
         * A container of items our player has offered to trade.
         */
        std::unique_ptr<ItemContainer> container_;

        /**
         * The amount of gold our player has offered to trade.
         */
        size_t gold_{ 0 };

        /**
         * If our player has confirmed the current trade window.
         */
        bool confirmed_{ false };
    };
}