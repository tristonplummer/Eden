#pragma once
#include <shaiya/common/crypto/Aes128Ctr.hpp>
#include <shaiya/common/net/Session.hpp>
#include <shaiya/login/net/EncryptionMode.hpp>
#include <shaiya/login/service/ServiceContext.hpp>

#include <glog/logging.h>

#include <array>
#include <crypto++/aes.h>
#include <crypto++/modes.h>

namespace shaiya::net
{
    /**
     * Represents a session that is connected to the login server.
     */
    class LoginSession: public Session
    {
    public:
        /**
         * Creates a new session from an io context.
         * @param ioContext The io context.
         * @param ctx       The service context
         */
        LoginSession(boost::asio::io_context& ioContext, shaiya::login::ServiceContext& ctx);

        /**
         * Writes a packet to this session's socket.
         * @tparam T        The packet type.
         * @param packet    The outgoing packet instance.
         * @param length    The length of the packet.
         */
        template<typename T>
        LoginSession& write(const T& packet, size_t length = sizeof(T))
        {
            if (encryptionMode_ == EncryptionMode::Encrypted)
            {
                encryption_.processData((byte*)&packet, length);
            }

            Session::write((const char*)&packet, length);
            return *this;
        }

        /**
         * This gets executed when the login session is accepted and connected to the server. This is used
         * to request a login handshake from the client.
         */
        void onAccept() override;

        /**
         * Initialises the encryption for this session.
         * @param key   The AES key.
         * @param iv    The AES iv.
         */
        void initEncryption(std::array<byte, 16> key, std::array<byte, 16> iv);

        /**
         * Sets the user id for this session.
         * @param userId    The user id.
         */
        void setUserId(uint32_t userId);

        /**
         * Gets the user id that this session was authenticated as.
         * @return  This session's user id
         */
        [[nodiscard]] uint32_t userId() const
        {
            return userId_;
        }

        /**
         * Gets the identity of this session.
         * @return  The identity.
         */
        [[nodiscard]] std::array<char, 16> identity() const
        {
            return identity_;
        }

        /**
         * Gets the AES key for this session.
         * @return  The key.
         */
        [[nodiscard]] std::array<uint8_t, 16> aesKey() const
        {
            return key_;
        }

        /**
         * Gets the AES iv for this session.
         * @return  The IV.
         */
        [[nodiscard]] std::array<uint8_t, 16> aesIv() const
        {
            return iv_;
        }

        /**
         * Gets the login service context.
         * @return  The context.
         */
        shaiya::login::ServiceContext& context() const
        {
            return ctx_;
        }

    private:
        /**
         * Gets executed when data is read from this session.
         * @param opcode    The opcode of the packet.
         * @param length    The length of the packet.
         * @param payload   The raw packet data.
         */
        void onRead(size_t opcode, size_t length, const char* payload) override;

        /**
         * The login service context.
         */
        shaiya::login::ServiceContext& ctx_;

        /**
         * The AES instance to use for outgoing packets.
         */
        shaiya::crypto::Aes128Ctr encryption_;

        /**
         * The AES instance to use for incoming packets.
         */
        shaiya::crypto::Aes128Ctr decryption_;

        /**
         * The user id that this session was authenticated as.
         */
        uint32_t userId_{ 0 };

        /**
         * The AES key
         */
        std::array<uint8_t, 16> key_{ 0 };

        /**
         * The AES iv
         */
        std::array<uint8_t, 16> iv_{ 0 };

        /**
         * The identity of this session.
         */
        std::array<char, 16> identity_{ 0 };

        /**
         * The encryption mode.
         */
        EncryptionMode encryptionMode_{ EncryptionMode::Plaintext };
    };
}