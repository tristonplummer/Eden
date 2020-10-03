#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for the account faction packet
     */
    constexpr auto AccountFactionOpcode = 0x0109;

    /**
     * A packet that informs the player's account of their account's faction.
     */
    struct AccountFactionNotify
    {
        /**
         * The opcode for the faction notification.
         */
        uint16_t opcode{ AccountFactionOpcode };

        /**
         * The faction of this account.
         */
        ShaiyaFaction faction{ ShaiyaFaction::Neither };

        /**
         * The maximum game mode that can be used when creating a new character.
         */
        ShaiyaGameMode maxGameMode{ ShaiyaGameMode::Ultimate };
    } PACKED;

    /**
     * Represents a request for an account to select their faction.
     */
    struct AccountFactionSelectRequest
    {
        /**
         * The opcode for the faction selection.
         */
        uint16_t opcode{ AccountFactionOpcode };

        /**
         * The selected faction value.
         */
        ShaiyaFaction faction{ ShaiyaFaction::Neither };
    } PACKED;
}