#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for sending a notice to a player.
     */
    constexpr auto NoticeOpcode = 0xF90B;

    /**
     * Represents the mob's state.
     */
    struct Notice
    {
        /**
         * The opcode of this notice.
         */
        uint16_t opcode{ NoticeOpcode };

        /**
         * The length of the message.
         */
        uint8_t length{};

        /**
         * The message.
         */
        VariableString<128> message;
    } PACKED;
}