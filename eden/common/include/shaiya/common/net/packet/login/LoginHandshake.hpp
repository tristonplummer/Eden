#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for a login handshake.
     */
    constexpr auto LoginHandshakeOpcode = 0xA101;

    /**
     * The length of the public RSA exponent.
     */
    constexpr auto ExponentLength = 64;

    /**
     * The length of the RSA modulus.
     */
    constexpr auto ModulusLength = 128;

    /**
     * Represents a request from the server to a client, signaling for the client
     * to begin a handshake and inform the server of the encryption seed that should
     * be used for further communication, which is to be encrypted using the provided
     * RSA public key.
     */
    struct LoginHandshakeRequest
    {
        /**
         * The opcode for the login handshake.
         */
        uint16_t opcode{ LoginHandshakeOpcode };

        /**
         * This is referred to as 'byEncMode' in the decompiled Ep4 files, and is
         * always set to 0.
         */
        NexonBoolean encrypted{ true };

        /**
         * The length of the public RSA exponent used by the client
         * for encrypting the response message.
         */
        uint8_t exponentLength{ ExponentLength };

        /**
         * The length of the RSA modulus used by both the server and the client.
         */
        uint8_t modulusLength{ ModulusLength };

        /**
         * The public RSA exponent.
         */
        std::array<char, ExponentLength> exponent{ 0 };

        /**
         * The RSA modulus.
         */
        std::array<char, ModulusLength> modulus{ 0 };
    } PACKED;

    /**
     * Represents the client's response to a handshake request. This response contains
     * an RSA encrypted message, which contains the seed used to generate the AES key
     * for further communications.
     */
    struct LoginHandshakeResponse
    {
        /**
         * The opcode for the login handshake.
         */
        uint16_t opcode{ LoginHandshakeOpcode };

        /**
         * The length of the encrypted message, which is equivalent
         * to the modulus length.
         */
        uint8_t messageLength{ ModulusLength };

        /**
         * The encrypted message data, which contains the RSA encrypted AES seed.
         */
        std::array<char, ModulusLength> message{ 0 };
    } PACKED;
}