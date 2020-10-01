#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode of the world list.
     */
    constexpr auto WorldListOpcode = 0xA201;

    /**
     * The status of the world server.
     */
    enum class WorldStatus : uint8_t
    {
        Normal,
        Locked,
        Closed
    };

    /**
     * An entry in the world list packet.
     */
    struct WorldListEntry
    {
        /**
         * The id of the world server
         */
        uint8_t id{ 0 };

        /**
         * The status of the world server.
         */
        WorldStatus status{ WorldStatus::Closed };

        /**
         * The number of players currently connected to the world.
         */
        uint16_t playerCount{ 0 };

        /**
         * The maximum number of players that can be connected to the world
         */
        uint16_t playerCapacity{ 0 };

        /**
         * The name of the world server
         */
        VariableString<32> name;
    } PACKED;

    /**
     * Represents the Shaiya world list packet.
     */
    template<size_t Count>
    struct WorldListPacket
    {
        /**
         * The opcode of the world list packet.
         */
        uint16_t opcode{ WorldListOpcode };

        /**
         * The number of world servers
         */
        uint8_t count{ 0 };

        /**
         * The world server entries
         */
        std::array<WorldListEntry, Count> servers;
    } PACKED;
}