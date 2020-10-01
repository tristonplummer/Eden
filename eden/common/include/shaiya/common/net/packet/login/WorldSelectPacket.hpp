#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode of the world select.
     */
    constexpr auto WorldSelectOpcode = 0xA202;

    /**
     * The status of a world selection request.
     */
    enum class WorldSelectStatus : int8_t
    {
        ServerSaturated    = -4,
        VersionDoesntMatch = -3,
        CannotConnect      = -2,
        TryAgainLater      = -1,
        Success            = 0
    };

    /**
     * Represents the Shaiya world select packet.
     */
    struct WorldSelectRequest
    {
        /**
         * The opcode of the world select packet.
         */
        uint16_t opcode{ WorldSelectOpcode };

        /**
         * The id of the selected server
         */
        uint8_t id{ 0 };

        /**
         * The client version
         */
        uint32_t version{ 0 };
    } PACKED;

    /**
     * Represents the response to a world selection request.
     */
    struct WorldSelectResponse
    {
        /**
         * The opcode of the world select packet.
         */
        uint16_t opcode{ WorldSelectOpcode };

        /**
         * The status of the login request.
         */
        WorldSelectStatus status{ WorldSelectStatus::CannotConnect };

        /**
         * The ip address of the server to connect to, if the status is positive
         */
        std::array<char, 4> ipAddress{ 0 };
    } PACKED;
}