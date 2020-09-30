#pragma once
#include <shaiya/common/net/packet/Packet.hpp>

#include <boost/asio.hpp>

#include <array>
#include <memory>
#include <vector>

namespace shaiya::net
{
    /**
     * Represents an active connection to the Shaiya tcp server.
     */
    class Session: public std::enable_shared_from_this<Session>
    {
    public:
        /**
         * Creates a new session from an io context.
         * @param context   The io context.
         */
        explicit Session(boost::asio::io_context& context);

        /**
         * Writes a packet to this session's socket.
         * @tparam T        The packet type.
         * @param packet    The outgoing packet instance.
         * @param length    The length of the packet.
         */
        virtual Session& write(const char* packet, size_t length)
        {
            // Write the packet to a buffer and allocate space for the size to be prefixed
            uint16_t packetLength = length + 2;
            std::vector<char> bytes;
            bytes.resize(packetLength);

            // Copy the packet and the length
            std::memcpy(bytes.data(), &packetLength, sizeof(packetLength));
            std::memcpy(bytes.data() + sizeof(packetLength), packet, length);

            // Write the outgoing packet
            auto handler = [bytes, session = shared_from_this()](const boost::system::error_code& error,
                                                                 size_t bytesTransferred) {
                session->handleWrite(error, bytesTransferred);
            };
            boost::asio::async_write(socket_, boost::asio::buffer(bytes.data(), bytes.size()), handler);
            return *this;
        }

        /**
         * Reads incoming data from this session.
         */
        void read();

        /**
         * Gracefully closes this session's connection.
         */
        void close();

        /**
         * Gets executed when this session is accepted by the server.
         */
        virtual void onAccept() = 0;

        /**
         * Gets executed when this session is disconnected.
         */
        virtual void onDisconnect();

        /**
         * Gets the socket for this session.
         * @return  The socket.
         */
        boost::asio::ip::tcp::socket& socket();

        /**
         * Gets the remote address of this session, in IPV4 format.
         * @return  The remote address.
         */
        std::string_view remoteAddress();

    private:
        /**
         * Handles a completed read event.
         * @param error             The error code returned.
         * @param bytesTransferred  The number of bytes that were read from the socket.
         */
        void handleRead(const boost::system::error_code& error, size_t bytesTransferred);

        /**
         * Handles the callback of a write event
         * @param error                 The returned error code
         * @param bytesTransferred      The number of bytes that were written to the socket
         */
        void handleWrite(const boost::system::error_code& error, size_t bytesTransferred);

        /**
         * Gets executed when a valid packet is read from the socket.
         * @param opcode    The opcode of the packet.
         * @param length    The length of the packet.
         * @param payload   The packet data.
         */
        virtual void onRead(size_t opcode, size_t length, const char* payload) = 0;

        /**
         * The remote socket of this session.
         */
        boost::asio::ip::tcp::socket socket_;

        /**
         * The remote ip address of this session.
         */
        std::string remoteAddress_;

        /**
         * The buffer to receive incoming packets into.
         */
        std::array<char, MAX_PACKET_LEN> buf_{ 0 };
    };
}