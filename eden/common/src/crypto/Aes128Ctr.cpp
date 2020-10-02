#include <shaiya/common/crypto/Aes128Ctr.hpp>

#include <glog/logging.h>

#include <crypto++/sha.h>

using namespace shaiya::crypto;

/**
 * The AES key size, in bytes.
 */
constexpr auto KEY_SIZE = CryptoPP::AES::DEFAULT_KEYLENGTH;

/**
 * The key size, in bits.
 */
constexpr auto KEY_SIZE_BITS = CryptoPP::AES::DEFAULT_KEYLENGTH * 8;

/**
 * Initialises this instance with a specified 128-bit key and iv.
 * @param key   The key.
 * @param iv    The counter.
 */
Aes128Ctr::Aes128Ctr(std::array<uint8_t, KEY_SIZE> key, std::array<uint8_t, KEY_SIZE> iv): key_(key), counter_(iv)
{
    // Initialise the counter encryptor
    counterEncryptor_.SetKey(key.data(), key.size());
}

/**
 * Operates on a set of data. This data does not need to be aligned to the block-size, nor does it need to be padded.
 * @param inout     The data to operate on.
 * @param length    The length of the data.
 */
void Aes128Ctr::processData(uint8_t* inout, size_t length)
{
    bool useExpandedKey = !expandedKey_.empty();
    for (auto i = 0; i < length; i++)
    {
        if (useExpandedKey)
        {
            inout[i] = (inout[i] ^ expandedKey_.at(i + length));
            continue;
        }

        if (xorMask_.empty())
            encryptThenIncrementCounter();

        auto mask = xorMask_.front();
        xorMask_.pop_front();
        inout[i] = (inout[i] ^ mask);
    }
}

/**
 * "Expands" the AES key to by recursively computing a SHA256 hash of the key, storing that in
 * an array, and then using that hash to generate the next key.
 */
void Aes128Ctr::expandKey()
{
    using namespace CryptoPP;

    // Create a SHA256 hash of an input key
    auto hash = [](const std::vector<uint8_t>& xorKey) -> std::vector<uint8_t> {
        std::vector<uint8_t> digest;
        digest.resize(SHA256::DIGESTSIZE);

        SHA256 sha256;
        sha256.CalculateDigest(digest.data(), xorKey.data(), xorKey.size());
        return digest;
    };

    // The initial expanded key is a hash based off the current AES key
    auto initialKey = std::vector<uint8_t>(key_.begin(), key_.end());
    auto keyDigest  = hash(initialKey);
    std::copy(keyDigest.begin(), keyDigest.end(), std::back_inserter(expandedKey_));

    // Expand the key
    for (auto i = 1; i <= KEY_SIZE_BITS; i++)
    {
        // Create a hash of the last 16 bytes of the expanded key
        std::vector<uint8_t> key(expandedKey_.end() - KEY_SIZE, expandedKey_.end());
        keyDigest = hash(key);
        std::copy(keyDigest.begin(), keyDigest.end(), std::back_inserter(expandedKey_));
    }
}

/**
 * Encrypts the counter with AES and uses the result to XOR the next block, and then increments the counter.
 */
void Aes128Ctr::encryptThenIncrementCounter()
{
    std::vector<uint8_t> counterBlock;
    counterBlock.resize(KEY_SIZE);

    counterEncryptor_.ProcessBlock(counter_.data(), counterBlock.data());

    incrementCounter();
    for (auto& mask: counterBlock)
        xorMask_.push_back(mask);
}

/**
 * Increments the counter value.
 */
void Aes128Ctr::incrementCounter()
{
    for (auto& i: counter_)
    {
        if (++i != 0)
            break;
    }
}