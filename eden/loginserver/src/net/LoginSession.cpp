#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/login/net/LoginSession.hpp>

using namespace shaiya::net;

/**
 * Creates a new session from an io context.
 * @param ioContext The io context.
 * @param ctx       The login service context.
 */
LoginSession::LoginSession(boost::asio::io_context& ioContext, shaiya::login::ServiceContext& ctx)
    : Session(ioContext), ctx_(ctx)
{
    // Generate this session's identity
    CryptoPP::AutoSeededRandomPool prng;
    prng.GenerateBlock((byte*) identity_.data(), identity_.size());
}

/**
 * This gets executed when the login session is accepted and connected to the server. This is used
 * to request a login handshake from the client.
 */
void LoginSession::onAccept()
{
    // Get the public key to use
    auto& key      = context().getEncryptionService().publicKey();
    auto& modulus  = key.GetModulus();
    auto& exponent = key.GetPublicExponent();

    // Encode the public key
    LoginHandshakeRequest request;
    request.modulusLength  = modulus.ByteCount();
    request.exponentLength = exponent.ByteCount();

    // The values needs to be in little-endian format
    modulus.Encode((byte*)request.modulus.data(), modulus.ByteCount(), CryptoPP::Integer::UNSIGNED);
    std::reverse(request.modulus.begin(), request.modulus.begin() + modulus.ByteCount());
    exponent.Encode((byte*)request.exponent.data(), exponent.ByteCount(), CryptoPP::Integer::UNSIGNED);
    std::reverse(request.exponent.begin(), request.exponent.begin() + exponent.ByteCount());
    write(request);
}

/**
 * Initialises the encryption for this session.
 * @param key   The AES key.
 * @param iv    The AES iv.
 */
void LoginSession::initEncryption(std::array<byte, 16> key, std::array<byte, 16> iv)
{
    // Set the encryption mode
    encryptionMode_ = EncryptionMode::Encrypted;
    encryption_     = shaiya::crypto::Aes128Ctr(key, iv);
    decryption_     = shaiya::crypto::Aes128Ctr(key, iv);
}

/**
 * Gets executed when data is read from this session.
 * @param opcode    The opcode of the packet.
 * @param length    The length of the packet.
 * @param payload   The raw packet data.
 */
void LoginSession::onRead(size_t opcode, size_t length, const char* payload)
{
    if (encryptionMode_ == EncryptionMode::Encrypted)
    {
        decryption_.processData((byte*)payload, length);  // Decrypt the inbound payload
        std::memcpy(&opcode, payload, sizeof(uint16_t));  // Copy the decrypted opcode
    }

    PacketRegistry::the().execute(*this, opcode, length, payload);
}