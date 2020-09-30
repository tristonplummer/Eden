#include <shaiya/login/service/EncryptionService.hpp>

#include <boost/filesystem.hpp>
#include <glog/logging.h>

#include <crypto++/base64.h>
#include <numeric>
#include <vector>

using namespace shaiya::login;

/**
 * Initialises this encryption service.
 */
EncryptionService::EncryptionService()
{
    using namespace CryptoPP;

    // The path to the private key
    boost::filesystem::path path("./data/cert/login.pem");
    if (!boost::filesystem::exists(path))
        return;

    // Load the private key
    readPemKey(path.string());
}

/**
 * Reads a PEM-encoded certificate.
 * @param path  The path to the certificate.
 */
void EncryptionService::readPemKey(const std::string& path)
{
    // Read the file lines into a vector of strings
    std::vector<std::string> lines;
    std::ifstream stream(path, std::ios::in);

    // Read the lines in the stream
    for (std::string line; std::getline(stream, line);)
    {
        if (line != "-----BEGIN RSA PRIVATE KEY-----" && line != "-----END RSA PRIVATE KEY-----")
            lines.push_back(line);
    }

    // The encoded key
    std::string encodedKey = std::accumulate(lines.begin(), lines.end(), std::string{});

    // Decode the key
    CryptoPP::ByteQueue queue;
    CryptoPP::StringSource source(encodedKey, true, new CryptoPP::Base64Decoder);
    source.TransferTo(queue);

    // Initialise the private key
    privateKey_.BERDecodePrivateKey(queue, false, 128);

    // Initialise public key
    publicKey_.Initialize(privateKey_.GetModulus(), privateKey_.GetPublicExponent());
}

/**
 * Decrypts a message with the RSA private key.
 * @param encrypted The encrypted message.
 * @return          The decrypted message.
 */
CryptoPP::Integer EncryptionService::decrypt(CryptoPP::Integer& encrypted)
{
    return privateKey_.CalculateInverse(prng_, encrypted);
}