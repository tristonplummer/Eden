#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for the XOR encryption signal.
     */
    constexpr auto XorEncryptionSignalOpcode = 0xB106;

    /**
     * A signal that notifies the server that we are ready to switch to XOR-based packet encryption, for packets that are
     * outgoing from the server to the client. This process doesn't actually start until we send the client their action bar,
     * presumably because it's the first packet the client receives that is large enough for regular AES decryption to be
     * slow.
     */
    struct XorEncryptionSignal
    {
        /**
         * The opcode of the XOR encryption signal.
         */
        uint16_t opcode{ XorEncryptionSignalOpcode };
    } PACKED;
}