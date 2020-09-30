#pragma once

#include <crypto++/files.h>
#include <crypto++/integer.h>
#include <crypto++/osrng.h>
#include <crypto++/rsa.h>

namespace shaiya::login
{
    /**
     * Handles the loading of a PEM-encoded RSA certificate, and the decryption of RSA-encoded messages.
     */
    class EncryptionService
    {
    public:
        /**
         * Initialises this encryption service.
         */
        EncryptionService();

        /**
         * Decrypts a message with the RSA private key.
         * @param encrypted The encrypted message.
         * @return          The decrypted message.
         */
        CryptoPP::Integer decrypt(CryptoPP::Integer& encrypted);

        /**
         * Gets the RSA public key.
         * @return  The public key.
         */
        [[nodiscard]] const CryptoPP::RSA::PublicKey& publicKey() const
        {
            return publicKey_;
        }

    private:
        /**
         * Reads a PEM-encoded certificate.
         * @param path  The path to the certificate.
         */
        void readPemKey(const std::string& path);

        /**
         * The RSA private key.
         */
        CryptoPP::RSA::PrivateKey privateKey_;

        /**
         * The RSA public key.
         */
        CryptoPP::RSA::PublicKey publicKey_;

        /**
         * The random number generator to use when decrypting messages.
         */
        CryptoPP::AutoSeededRandomPool prng_;
    };
}