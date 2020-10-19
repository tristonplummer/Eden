#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/net/packet/game/CharacterTradeRequest.hpp>

namespace shaiya::net
{
    /**
     * The opcode for adding gold to a trade session.
     */
    constexpr auto TradeAddGoldOpcode = 0x0A08;

    /**
     * Represents a request for the player to add gold to their half of the trade window.
     */
    struct CharacterTradeOfferGoldRequest
    {
        /**
         * The added gold opcode.
         */
        uint16_t opcode{ TradeAddGoldOpcode };

        /**
         * The amount of gold that the player would like to offer.
         */
        uint32_t gold{ 0 };
    } PACKED;

    /**
     * Represents an update about the gold present in a trade session.
     */
    struct CharacterTradeGoldUpdate
    {
        /**
         * The added gold opcode.
         */
        uint16_t opcode{ TradeAddGoldOpcode };

        /**
         * The participant that we're updating the gold for.
         */
        Participant participant{ Participant::Self };

        /**
         * The current amount of gold on offer by the participant.
         */
        uint32_t gold{ 0 };
    } PACKED;
}