#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for a game handshake.
     */
    constexpr auto GameHandshakeOpcode = 0xA301;

    /**
     * Represents a request from a client that has just disconnected from the login server, to authenticate
     * itself with this game server.
     */
    struct GameHandshakeRequest
    {
        /**
         * The opcode for the game handshake.
         */
        uint16_t opcode{ GameHandshakeOpcode };

        /**
         * The user id that was provided to the client by the login server.
         */
        uint32_t userId{ 0 };

        /**
         * The identity token
         */
        std::array<char, 16> identity{ 0 };
    } PACKED;

    /**
     * Represents the response to a handshake request. This response is used to inform the client about
     * whether or not they have been successfully authenticated to the game server, and to provide the client
     * with the AES key that should be used for further communication.
     */
    struct GameHandshakeResponse
    {
        /**
         * The opcode for the game handshake.
         */
        uint16_t opcode{ GameHandshakeOpcode };

        /**
         * If the handshake wa a success.
         */
        NexonBoolean success{ true };

        /**
         * Not entirely sure what each of the values represent here, though we
         * do know that the Ep4 server files only ever use the value 2.
         */
        uint8_t encryptionMode{ 2 };

        /**
         * The AES key to use for further packet exchanges.
         */
        std::array<char, 16> expandedKeySeed;
    } PACKED;
}