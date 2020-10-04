#pragma once
#include <shaiya/common/util/Struct.hpp>

namespace shaiya
{
    /**
     * Listen. In every programmer's life, there comes a time where you need to make a choice. Do you do what you know is
     * right, or do you give in to your insanity, and intentionally write code that you know is twisted and morally wrong.
     * This my friends, is that code. It's an inverted boolean, because for whatever reason, Nexon likes to do things the
     * opposite.
     *
     * 0 = True
     * Non-zero = False
     */
    class NexonBoolean
    {
    public:
        /**
         * Initialises this boolean with a C++ boolean, but flips the value.
         * @param value The value.
         */
        NexonBoolean(bool value)
        {
            data_ = value ? 0 : 1;
        }

        /**
         * The boolean operator of this structure. Returns true if the value is 0.
         * @return  The boolean value.
         */
        explicit operator bool() const
        {
            return data_ == 0;
        }

    private:
        /**
         * The internal value. The default is 1, which is a false value.
         */
        uint8_t data_{ 1 };

    } PACKED;
}