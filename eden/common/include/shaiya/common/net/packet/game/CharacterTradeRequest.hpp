#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for a trade request.
     */
    constexpr auto TradeRequestOpcode = 0x0A01;

    /**
     * The opcode for a response to a trade request.
     */
    constexpr auto TradeResponseOpcode = 0x0A02;

    /**
     * The opcode for initiating a trade screen.
     */
    constexpr auto TradeInitiateOpcode = 0x0A03;

    /**
     * The opcode for cancelling a trade session.
     */
    constexpr auto TradeCancelledOpcode = 0x0A04;

    /**
     * The opcode for finalising a trade.
     */
    constexpr auto TradeFinaliseOpcode = 0x0A05;

    /**
     * Represents the finalisation cause of a trade.
     */
    enum class TradeFinaliseType : uint8_t
    {
        Cancel = 2
    };

    /**
     * Represents a participant in a trading session.
     */
    enum class Participant : uint8_t
    {
        Self    = 1,
        Partner = 2
    };

    /**
     * Represents a request to open a trade window with another player.
     */
    struct CharacterTradeRequest
    {
        /**
         * The opcode of the trade request.
         */
        uint16_t opcode{ TradeRequestOpcode };

        /**
         * The id of the target player.
         */
        uint32_t target{ 0 };
    } PACKED;

    /**
     * Represents a response to a trade request.
     */
    struct CharacterTradeResponse
    {
        /**
         * The opcode of the trade response.
         */
        uint16_t opcode{ TradeResponseOpcode };

        /**
         * If the trade was accepted.
         */
        NexonBoolean accepted{ false };
    } PACKED;

    /**
     * Represents the initiation of a trade between two players.
     */
    struct TradeInitiate
    {
        /**
         * The opcode for the trade initiation.
         */
        uint16_t opcode{ TradeInitiateOpcode };

        /**
         * The character id of the trading partner.
         */
        uint32_t partner{ 0 };
    } PACKED;

    /**
     * Represents a request to finalise a trade.
     */
    struct CharacterTradeFinaliseRequest
    {
        /**
         * The opcode for the finalisation.
         */
        uint16_t opcode{ TradeFinaliseOpcode };

        /**
         * The finalisation reason.
         */
        TradeFinaliseType type{ TradeFinaliseType::Cancel };
    } PACKED;

    /**
     * Represents a notification that an open trade session was cancelled.
     */
    struct CharacterTradeCancelled
    {
        /**
         * The opcode of the trade cancellation.
         */
        uint16_t opcode{ TradeCancelledOpcode };

        /**
         * The player who closed the trade.
         */
        Participant participant{ Participant::Partner };
    } PACKED;

}