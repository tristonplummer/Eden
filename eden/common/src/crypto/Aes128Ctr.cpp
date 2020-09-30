#include <shaiya/common/crypto/Aes128Ctr.hpp>

#include <glog/logging.h>

using namespace shaiya::crypto;

/**
 * Initialises this instance with a specified 128-bit key and iv.
 * @param key   The key.
 * @param iv    The counter.
 */
Aes128Ctr::Aes128Ctr(std::array<uint8_t, 16> key, std::array<uint8_t, 16> iv): key_(key), counter_(iv)
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
    for (auto i = 0; i < length; i++)
    {
        if (xorMask_.empty())
            encryptThenIncrementCounter();

        auto mask = xorMask_.front();
        xorMask_.pop_front();
        inout[i] = (inout[i] ^ mask);
    }
}

/**
 * Encrypts the counter with AES and uses the result to XOR the next block, and then increments the counter.
 */
void Aes128Ctr::encryptThenIncrementCounter()
{
    std::vector<uint8_t> counterBlock;
    counterBlock.resize(counter_.size());

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