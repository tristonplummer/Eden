#pragma once

#include <array>
#include <crypto++/aes.h>
#include <deque>

namespace shaiya::crypto
{
    /**
     * A simple implementation of AES-CTR mode with a 128-bit key.
     * Derived from https://gist.github.com/hanswolff/8809275
     */
    class Aes128Ctr
    {
    public:
        /**
         * Default initialises with a zero kev and counter.
         */
        Aes128Ctr() = default;

        /**
         * Initialises this instance with a specified 128-bit key and iv.
         * @param key   The key.
         * @param iv    The counter.
         */
        Aes128Ctr(std::array<uint8_t, 16> key, std::array<uint8_t, 16> iv);

        /**
         * Operates on a set of data. This data does not need to be aligned to the block-size, nor does it need to be padded.
         * @param inout     The data to operate on.
         * @param length    The length of the data.
         */
        void processData(uint8_t* inout, size_t length);

        /**
         * "Expands" the AES key to by recursively computing a SHA256 hash of the key, storing that in
         * an array, and then using that hash to generate the next key.
         */
        void expandKey();

    private:
        /**
         * The AES key.
         */
        std::array<uint8_t, 16> key_{ 0 };

        /**
         * The counter value.
         */
        std::array<uint8_t, 16> counter_{ 0 };

        /**
         * The expanded key value.
         */
        std::vector<uint8_t> expandedKey_;

        /**
         * The values used to XOR-encrypt the current block.
         */
        std::deque<uint8_t> xorMask_;

        /**
         * The AES instance used to encrypt the counter block.
         */
        CryptoPP::AES::Encryption counterEncryptor_;

        /**
         * Encrypts the counter with AES and uses the result to XOR the next block, and then increments the counter.
         */
        void encryptThenIncrementCounter();

        /**
         * Increments the counter value.
         */
        void incrementCounter();
    };
}