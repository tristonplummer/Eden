#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/login/net/LoginSession.hpp>

#include <crypto++/hmac.h>
#include <sstream>

using namespace shaiya::net;

/**
 * Handles an incoming handshake response.
 * @param session   The session instance
 * @param request   The handshake response.
 */
void handleHandshakeResponse(Session& session, const LoginHandshakeResponse& response)
{
    using namespace CryptoPP;
    auto& login      = dynamic_cast<LoginSession&>(session);  // The login session instance.
    auto& encryption = login.context().getEncryptionService();

    // Convert the message to a Crypto++ Integer
    assert(response.messageLength == ModulusLength);
    Integer encrypted((byte*)response.message.data(), ModulusLength, CryptoPP::Integer::UNSIGNED, LITTLE_ENDIAN_ORDER);

    // Decrypt the response
    std::vector<byte> decrypted;
    auto d = encryption.decrypt(encrypted);
    decrypted.resize(d.MinEncodedSize(CryptoPP::Integer::SIGNED));
    d.Encode(decrypted.data(), decrypted.size());
    std::reverse(decrypted.begin(), decrypted.end());

    // Get the modulus
    auto& modulus = encryption.publicKey().GetModulus();
    std::vector<byte> n;
    n.resize(modulus.ByteCount());
    modulus.Encode(n.data(), n.size(), CryptoPP::Integer::UNSIGNED);
    std::reverse(n.begin(), n.end());

    // Calculate a HMAC-256 based off the response, and the modulus
    std::array<byte, HMAC<SHA256>::DIGESTSIZE> digest{ 0 };
    HMAC<SHA256> hmac(decrypted.data(), decrypted.size());
    hmac.CalculateDigest(digest.data(), n.data(), n.size());

    // The key and iv
    std::array<byte, 16> key{ 0 };
    std::array<byte, 16> iv{ 0 };

    // Copy the key and iv
    std::memcpy(key.data(), &digest[0], key.size());
    std::memcpy(iv.data(), &digest[key.size()], iv.size());

    // Initialise the session's encryption
    login.initEncryption(key, iv);
}

/**
 * A template specialization used for registering the handshake response packet handler.
 */
template<>
void PacketRegistry::registerPacketHandler<LoginHandshakeOpcode>()
{
    registerHandler<LoginHandshakeOpcode, LoginHandshakeResponse>(&handleHandshakeResponse, ExecutionType::Asynchronous);
}