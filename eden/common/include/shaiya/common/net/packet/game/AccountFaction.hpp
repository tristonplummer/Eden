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
         * The opcode for the game handshake.
         */
        uint16_t opcode{ AccountFactionOpcode };

        /**
         * 0 = Light
         * 1 = Fury
         * 2 = Undecided
         */
        uint8_t faction{ 2 };

        /**
         * The maximum game mode that can be used when creating a new character.
         *
         * 0 = Easy
         * 1 = Normal
         * 2 = Hard
         * 3 = Ultimate
         */
        uint8_t maxGameMode{ 3 };
    } PACKED;
}