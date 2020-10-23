#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/net/packet/game/CharacterItem.hpp>
#include <shaiya/common/net/packet/game/CharacterTradeRequest.hpp>

namespace shaiya::net
{
    /**
     * The opcode for offering an item to trade.
     */
    constexpr auto TradeOfferItemOpcode = 0x0A06;

    /**
     * The opcode for an item being removed from the trade window.
     */
    constexpr auto TradeRemoveItemOpcode = 0x0A07;

    /**
     * The opcode for a trade partner offering an item.
     */
    constexpr auto TradePartnerOfferItemOpcode = 0x0A09;

    /**
     * Represents a request for a player to offer an item up for trade.
     */
    struct CharacterTradeOfferItemRequest
    {
        /**
         * The trade offer opcode.
         */
        uint16_t opcode{ TradeOfferItemOpcode };

        /**
         * The page in the inventory to offer from.
         */
        uint8_t page{ 0 };

        /**
         * The slot in the inventory to offer from.
         */
        uint8_t slot{ 0 };

        /**
         * The quantity of the item to offer.
         */
        uint8_t quantity{ 0 };

        /**
         * The destination slot in the trade window.
         */
        uint8_t destSlot{ 0 };
    } PACKED;

    /**
     * Represents a request to remove an item from the player's trade screen.
     */
    struct CharacterTradeRemoveItemRequest
    {
        /**
         * The opcode for removing an item.
         */
        uint16_t opcode{ TradeRemoveItemOpcode };

        /**
         * The slot to remove the item from.
         */
        uint8_t slot{ 0 };
    } PACKED;

    /**
     * Represents an item being removed from the trade screen.
     */
    struct CharacterTradeRemoveItem
    {
        /**
         * The opcode for removing an item.
         */
        uint16_t opcode{ TradeRemoveItemOpcode };

        /**
         * The participant in the trade.
         */
        Participant participant{ Participant::Self };

        /**
         * The slot to remove the item from.
         */
        uint8_t slot{ 0 };
    } PACKED;

    /**
     * Represents an update that the current trading partner has offered an item.
     */
    struct CharacterTradePartnerOfferItem
    {
        /**
         * The trade offer opcode.
         */
        uint16_t opcode{ TradePartnerOfferItemOpcode };

        /**
         * The slot in the trade window.
         */
        uint8_t slot{ 0 };

        /**
         * The type of the item.
         */
        uint8_t type{ 0 };

        /**
         * The type id of the item.
         */
        uint8_t typeId{ 0 };

        /**
         * The quantity of the item.
         */
        uint8_t quantity{ 0 };

        /**
         * The endurance of the item.
         */
        uint16_t endurance{ 0 };

        /**
         * The time that the item was created.
         */
        ShaiyaTime creation;

        /**
         * The time that the item will expire.
         */
        ShaiyaTime expiration;

        /**
         * TODO: Refactor this.
         */
        VariableString<22> unknown;

        /**
         * If the item is dyed.
         */
        bool dyed{ false };

        /**
         * TODO: Refactor this.
         */
        VariableString<26> unknown2;

        /**
         * The item ids of the lapis that are socketed into this item.
         */
        std::array<uint32_t, LapisCapacity> lapis{ 0 };

        /**
         * The craftname of the item.
         */
        VariableString<21> craftname;
    } PACKED;
}