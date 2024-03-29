#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/service/ServiceContext.hpp>

#include <crypto++/sha.h>

using namespace shaiya::net;

/**
 * Creates a new session from an io context.
 * @param ioContext The io context.
 * @param ctx       The login service context.
 */
GameSession::GameSession(boost::asio::io_context& ioContext, shaiya::game::ServiceContext& ctx)
    : Session(ioContext), ctx_(ctx)
{
}

/**
 * This gets executed when the game session is accepted and connected to the server.
 */
void GameSession::onAccept()
{
}

/**
 * Initialises the encryption for this session.
 * @param key       The AES key.
 * @param iv        The AES iv.
 * @param xorKey    The XOR key to use in expanded encryption.
 */
void GameSession::initEncryption(std::array<byte, 16> key, std::array<byte, 16> iv, std::array<byte, 16> xorKey)
{
    using namespace CryptoPP;

    // Save the keys and the iv
    xorKey_ = xorKey;
    key_    = key;
    iv_     = iv;

    // Generate a hash based off the IV
    std::vector<uint8_t> digest;
    digest.resize(SHA256::DIGESTSIZE);
    SHA256 sha256;
    sha256.CalculateDigest(digest.data(), iv_.data(), iv_.size());

    // Copy the first 16 bytes of the hash to the iv
    std::memcpy(iv_.data(), digest.data(), iv_.size());

    // Set the encryption mode
    encryptionMode_ = EncryptionMode::Encrypted;
    encryption_     = shaiya::crypto::Aes128Ctr(key_, iv_);
    decryption_     = shaiya::crypto::Aes128Ctr(key_, iv_);
}

/**
 * Initialises the XOR encryption for this session.
 */
void GameSession::initXorEncryption()
{
    encryption_ = shaiya::crypto::Aes128Ctr(xorKey_, iv_);
    encryption_.expandKey();
}

/**
 * Sets the user id for this session.
 * @param userId    The user id.
 */
void GameSession::setUserId(uint32_t userId)
{
    userId_ = userId;
}

/**
 * Sets the faction for this session.
 * @param faction   The faction value.
 */
void GameSession::setFaction(ShaiyaFaction faction)
{
    faction_ = faction;
}

/**
 * Sets the character instance for this session.
 * @param player The character.
 */
void GameSession::setPlayer(std::shared_ptr<shaiya::game::Player> player)
{
    player_ = std::move(player);
}

/**
 * Gets executed when this session gets disconnected.
 */
void GameSession::onDisconnect()
{
    if (player_)
    {
        auto& world = context().getGameWorld();
        world.unregisterPlayer(player_);
    }

    player_.reset();
}

/**
 * Processes the queue of packets that are yet to be processed.
 */
void GameSession::processQueue()
{
    // Lock the mutex
    std::lock_guard lock{ mutex_ };

    // Loop over the queued packets
    while (!queuedPackets_.empty())
    {
        auto packet = queuedPackets_.front();
        queuedPackets_.pop_front();

        // The packet data and the opcode
        auto* data  = packet.data();
        auto opcode = *reinterpret_cast<uint16_t*>(data);

        // Process the packet
        PacketRegistry::the().execute(*this, opcode, packet.size(), data);
    }
}

/**
 * Gets executed when data is read from this session.
 * @param opcode    The opcode of the packet.
 * @param length    The length of the packet.
 * @param payload   The raw packet data.
 */
void GameSession::onRead(size_t opcode, size_t length, const char* payload)
{
    using namespace shaiya::net;
    if (encryptionMode_ == EncryptionMode::Encrypted)
    {
        decryption_.processData((byte*)payload, length);  // Decrypt the inbound payload
        std::memcpy(&opcode, payload, sizeof(uint16_t));  // Copy the decrypted opcode
    }

    // Only handshake and character-screen packets should be executed asynchronously
    if (isType(opcode, PacketType::Handshake) || isType(opcode, PacketType::CharacterScreen))
    {
        PacketRegistry::the().execute(*this, opcode, length, payload);
        return;
    }

    // Lock the mutex
    std::lock_guard lock{ mutex_ };

    // Queue the packet
    std::vector<char> packet(payload, payload + length);
    queuedPackets_.push_back(packet);
}