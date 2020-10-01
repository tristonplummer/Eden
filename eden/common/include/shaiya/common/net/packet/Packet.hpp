#pragma once
#include <cassert>
#include <cstdint>
#include <cstring>

namespace shaiya::net
{
    /**
     * The maximum length that we should accept for incoming packets. While this technically
     * could be the maximum size limit of the length short (0xFFFF), we want to make this value
     * as small as possible, because we don't want to be having to read or copy large chunks
     * of incoming data.
     */
    constexpr auto MAX_PACKET_LEN = 256;

    /**
     * Constructs a packet structure instance from a source byte buffer.
     * @tparam T        The packet type.
     * @param data      The data array.
     * @param length    The length of the data array.
     * @param copy      If the data should be copied rather than cast directly to a structure.
     * @return          The constructed packet.
     */
    template<typename T>
    inline T toPacket(const char* data, size_t length = sizeof(T))
    {
        assert(data);                      // Assert that we actually have data.
        assert(length >= 2);               // Assert that the length at least includes an opcode.
        assert(MAX_PACKET_LEN >= length);  // Assert that the packet isn't too large.

        T packet;
        std::memcpy(&packet, data, length);
        return packet;
    }
}

// Include the login packets
#include "login/AccountLoginRequest.hpp"
#include "login/LoginHandshake.hpp"
#include "login/WorldListPacket.hpp"
#include "login/WorldSelectPacket.hpp"