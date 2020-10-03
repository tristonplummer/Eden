#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/game/net/GameSession.hpp>

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
 * @param key   The AES key.
 * @param iv    The AES iv.
 */
void GameSession::initEncryption(std::array<byte, 16> key, std::array<byte, 16> iv)
{
    using namespace CryptoPP;

    // Save the key and iv
    key_ = key;
    iv_  = iv;

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
 * Shows the character screen data to this screen.
 */
void GameSession::showCharacterScreen()
{
    // Send the player their faction
    shaiya::net::AccountFactionNotify faction;
    write(faction);

    // Send the empty character list
    for (auto i = 0; i < 5; i++)
    {
        shaiya::net::CharacterListEntry entry;
        entry.slot = i;
        write(entry);
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
    if (encryptionMode_ == EncryptionMode::Encrypted)
    {
        decryption_.processData((byte*)payload, length);  // Decrypt the inbound payload
        std::memcpy(&opcode, payload, sizeof(uint16_t));  // Copy the decrypted opcode
    }

    PacketRegistry::the().execute(*this, opcode, length, payload);
}