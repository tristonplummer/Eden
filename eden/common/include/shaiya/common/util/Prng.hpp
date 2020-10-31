#pragma once
#include <random>

namespace shaiya
{
    /**
     * A simple class to help generate random numbers within a Shaiya context. This is simply because the initialisation
     * of random devices is expensive, and I'd prefer to only have to do it once within an application's lifetime.
     */
    class Prng
    {
    public:
        /**
         * Initialises the prng.
         */
        Prng(): prng_(rd_())
        {
        }

        /**
         * Gets the PRNG singleton.
         * @return  The prng.
         */
        static Prng& the()
        {
            static Prng prng;
            return prng;
        }

        /**
         * Gets a random value between a minimum and maximum
         * @tparam T    The type.
         * @param min   The minimum value
         * @param max   The maximum value (inclusive)
         * @return      The random value
         */
        template<typename T>
        T random(T min, T max)
        {
            return random(min, max, T{});
        }

        /**
         * Gets a random value between a minimum and maximum, with a provided range.
         * @tparam T        The type.
         * @param min       The minimum value.
         * @param max       The maximum value.
         * @param range     The range.
         * @return          The random value.
         */
        template<typename T>
        T random(T min, T max, T range)
        {
            std::uniform_real_distribution<T> dist(min - range, max + range);
            return dist(prng_);
        }

        /**
         * Checks if a random value matches a percentage
         * @param percent   The required percentage
         * @return          If the percentage is matched
         */
        bool percentage(uint32_t percent)
        {
            std::uniform_int_distribution<> dist(0, 100);
            return dist(prng_) < percent;
        }

    private:
        /**
         * The random device.
         */
        std::random_device rd_;

        /**
         * The PRNG implementation.
         */
        std::mt19937 prng_;
    };
}