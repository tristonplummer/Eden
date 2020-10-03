#pragma once

namespace shaiya::net
{
    /**
     * The types of packets.
     */
    enum PacketType
    {
        /**
         * Represents a handshake related packet.
         */
        Handshake = 0xA300,

        /**
         * Represents the packets that are sent at the character screen.
         */
        CharacterScreen = 0x0100
    };
}