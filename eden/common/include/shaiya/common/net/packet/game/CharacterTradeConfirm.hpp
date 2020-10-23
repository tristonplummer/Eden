#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/net/packet/game/CharacterTradeRequest.hpp>

namespace shaiya::net
{
    /**
     * The opcode for confirming a trade session.
     */
    constexpr auto TradeConfirmOpcode = 0x0A0A;

    /**
     * Represents a request by a player to confirm their trade session.
     */
    struct CharacterConfirmTrade
    {
        /**
         * The confirmation opcode.
         */
        uint16_t opcode{ TradeConfirmOpcode };

        /**
         * If the trade was confirmed.
         */
        bool confirmed{ false };
    } PACKED;

    /**
     * Represents an update about a participant's confirmation status.
     */
    struct CharacterConfirmTradeUpdate
    {
        /**
         * The confirmation opcode.
         */
        uint16_t opcode{ TradeConfirmOpcode };

        /**
         * The participant who is has either confirmed or unconfirmed the trade.
         */
        Participant participant{ Participant::Self };

        /**
         * If the trade was confirmed.
         */
        bool confirmed{ false };
    } PACKED;
}