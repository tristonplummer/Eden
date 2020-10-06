#pragma once
#include <shaiya/common/crypto/Aes128Ctr.hpp>
#include <shaiya/common/net/Session.hpp>
#include <shaiya/game/net/EncryptionMode.hpp>
#include <shaiya/game/service/ServiceContext.hpp>
#include <shaiya/game/world/model/actor/character/Character.hpp>

#include <glog/logging.h>

#include <array>
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <deque>
#include <mutex>
#include <vector>

namespace shaiya::net
{
    /**
     * Represents a session that is connected to the game server.
     */
    class GameSession: public Session
    {
    public:
        /**
         * Creates a new session from an io context.
         * @param ioContext The io context.
         * @param ctx       The service context
         */
        GameSession(boost::asio::io_context& ioContext, shaiya::game::ServiceContext& ctx);

        /**
         * Writes a packet to this session's socket.
         * @tparam T        The packet type.
         * @param packet    The outgoing packet instance.
         * @param length    The length of the packet.
         */
        template<typename T>
        GameSession& write(const T& packet, size_t length = sizeof(T))
        {
            if (encryptionMode_ == EncryptionMode::Encrypted)
            {
                encryption_.processData((byte*)&packet, length);
            }

            Session::write((const char*)&packet, length);
            return *this;
        }

        /**
         * This gets executed when the game session is accepted and connected to the server.
         */
        void onAccept() override;

        /**
         * Initialises the encryption for this session.
         * @param key       The AES key.
         * @param iv        The AES iv.
         * @param xorKey    The XOR key to use in expanded encryption.
         */
        void initEncryption(std::array<byte, 16> key, std::array<byte, 16> iv, std::array<byte, 16> xorKey);

        /**
         * Initialises the XOR encryption for this session.
         */
        void initXorEncryption();

        /**
         * Processes the queue of packets that are yet to be processed.
         */
        void processQueue();

        /**
         * Sets the user id for this session.
         * @param userId    The user id.
         */
        void setUserId(uint32_t userId);

        /**
         * Sets the faction for this session.
         * @param faction   The faction value.
         */
        void setFaction(ShaiyaFaction faction);

        /**
         * Gets the user id that this session was authenticated as.
         * @return  This session's user id
         */
        [[nodiscard]] uint32_t userId() const
        {
            return userId_;
        }

        /**
         * Gets the faction of this user.
         * @return  The faction.
         */
        [[nodiscard]] ShaiyaFaction faction() const
        {
            return faction_;
        }

        /**
         * Gets the game service context.
         * @return  The context.
         */
        shaiya::game::ServiceContext& context() const
        {
            return ctx_;
        }

        /**
         * Sets the character instance for this session.
         * @param character The character.
         */
        void setCharacter(std::shared_ptr<shaiya::game::Character> character);

        /**
         * Gets executed when this session gets disconnected.
         */
        void onDisconnect() override;

        /**
         * Gets the character associated with this session.
         * @return  The character instance.
         */
        std::shared_ptr<shaiya::game::Character> character()
        {
            return character_;
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
         * The game service context.
         */
        shaiya::game::ServiceContext& ctx_;

        /**
         * The user id that this session was authenticated as.
         */
        uint32_t userId_{ 0 };

        /**
         * The faction of this session.
         */
        ShaiyaFaction faction_{ ShaiyaFaction::Neither };

        /**
         * The character associated with this session.
         */
        std::shared_ptr<shaiya::game::Character> character_{ nullptr };

        /**
         * The XOR key to use in expanded encryption.
         */
        std::array<uint8_t, 16> xorKey_{ 0 };

        /**
         * The AES key
         */
        std::array<uint8_t, 16> key_{ 0 };

        /**
         * The AES iv
         */
        std::array<uint8_t, 16> iv_{ 0 };

        /**
         * The AES instance to use for outgoing packets.
         */
        shaiya::crypto::Aes128Ctr encryption_;

        /**
         * The AES instance to use for incoming packets.
         */
        shaiya::crypto::Aes128Ctr decryption_;

        /**
         * The encryption mode.
         */
        EncryptionMode encryptionMode_{ EncryptionMode::Plaintext };

        /**
         * The mutex to be used for locking access to the packet queue.
         */
        std::mutex mutex_;

        /**
         * The queue of packets that are yet to be processed.
         */
        std::deque<std::vector<char>> queuedPackets_;
    };
}