#pragma once
#include <shaiya/common/util/Struct.hpp>

#include <chrono>

namespace shaiya
{
    /**
     * Represents a point in time, encoded in Shaiya's format as a 32-bit integer.
     */
    class ShaiyaTime
    {
    public:
        /**
         * The default constructor just initialises the internal time to 0.
         */
        ShaiyaTime() = default;

        /**
         * Constructs a Shaiya time instance from a time point.
         * @param point The time point.
         * @return      The current time.
         */
        ShaiyaTime(std::chrono::system_clock::time_point point)
        {
            auto time = std::chrono::system_clock::to_time_t(point);
            encode(localtime(&time));
        }

        /**
         * Decodes this Shaiya time into a time structure.
         * @param time  The time struct.
         */
        [[nodiscard]] std::tm decode() const
        {
            std::tm time{};
            time.tm_year = ((time_ >> 26u) & 0x3Fu) + 2000;
            time.tm_mon  = (time_ >> 22u) & 0xFu;
            time.tm_mday = (time_ >> 17u) & 0x1Fu;
            time.tm_hour = (time_ >> 12u) & 0x1Fu;
            time.tm_min  = (time_ >> 6) & 0x3Fu;
            time.tm_sec  = time_ & 0x3Fu;
            return time;
        }

    private:
        /**
         * Encodes a time point into this Shaiya time structure.
         * @param time  The time.
         */
        void encode(const std::tm* time)
        {
            time_ = time->tm_sec +
                    ((time->tm_min +
                      ((time->tm_hour + 32 * (time->tm_mday + 32 * ((time->tm_mon + 1) + 16 * (time->tm_year - 16)))) << 6))
                     << 6);
        }

        /**
         * The internal time value, used as an integer in the Shaiya protocol.
         */
        uint32_t time_{ 0 };
    } PACKED;
}