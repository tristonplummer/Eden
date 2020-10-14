#pragma once
#include <shaiya/common/DataTypes.hpp>

#include <chrono>

namespace shaiya::net
{
    /**
     * The opcode for informing a player about the current time.
     */
    constexpr auto WorldTimeOpcode = 0x0114;

    /**
     * Informs a player about the current local world time.
     */
    struct WorldTime
    {
        /**
         * The opcode for informing a player about the time.
         */
        uint16_t opcode{ WorldTimeOpcode };

        /**
         * The current time.
         */
        ShaiyaTime time{ std::chrono::system_clock::now() };
    } PACKED;
}