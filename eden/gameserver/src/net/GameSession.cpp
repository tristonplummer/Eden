#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/game/net/GameSession.hpp>

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
    // Set the encryption mode
    encryptionMode_ = EncryptionMode::Encrypted;
    encryption_     = shaiya::crypto::Aes128Ctr(key, iv);
    decryption_     = shaiya::crypto::Aes128Ctr(key, iv);
}

/**
 * Shows the character screen data to this screen.
 */
void GameSession::showCharacterScreen()
{
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