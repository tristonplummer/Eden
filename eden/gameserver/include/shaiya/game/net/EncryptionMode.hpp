#pragma once

namespace shaiya::net
{
    /**
     * Represents an encryption mode for a game session.
     */
    enum class EncryptionMode
    {
        /**
         * Packets should be left as is, with no encryption
         * or decryption.
         */
        Plaintext,

        /**
         * Packets should be encrypted and decrypted with AES128-CTR
         */
        Encrypted
    };
}