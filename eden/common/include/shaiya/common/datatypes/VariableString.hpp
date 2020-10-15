#pragma once
#include <shaiya/common/util/Struct.hpp>

#include <array>
#include <string>

namespace shaiya
{
    /**
     * Represents a string with a specific maximum length. This is nothing more
     * than a simple fixed-size character array, with some helper functions.
     * @tparam MaxLen   The maximum length of the string.
     */
    template<size_t MaxLen>
    class VariableString
    {
    public:
        /**
         * The default constructor
         */
        VariableString() = default;

        /**
         * Initialise this variable string from a C++ string.
         * @param string    The input string.
         */
        VariableString(const std::string& string)
        {
            read(string.c_str());
        }

        /**
         * Initialise this variable string from a character array.
         * @param string    The input array.
         */
        VariableString(const char* string)
        {
            read(string);
        }

        /**
         * Reads a null-terminated string from an array of characters.
         * @param str  The character array to read from.
         */
        void read(const char* str)
        {
            for (auto i = 0; i < MaxLen; i++)
            {
                if (str[i] == '\0')
                    break;
                data_.at(i) = str[i];
            }
        }

        /**
         * Reads this variable string as a C++ string view.
         * @return  The string.
         */
        [[nodiscard]] std::string str() const
        {
            return std::string(data_.data());
        }

        /**
         * Provides an implicit conversion to a string.
         * @return  The string
         */
        [[nodiscard]] operator std::string() const
        {
            return str();
        }

        /**
         * Provides an implicit conversion to const char*
         * @return  The char array
         */
        [[nodiscard]] operator const char*() const
        {
            return data_.data();
        }

        /**
         * Checks if another string contains the same data as this one.
         * @param other The other string.
         * @return      If the two strings match.
         */
        [[nodiscard]] bool operator==(const VariableString<MaxLen>& other) const
        {
            for (auto i = 0; i < MaxLen; i++)
            {
                if (data_.at(i) != other.data_.at(i))
                    return false;
            }
            return true;
        }
    private:
        /**
         * The internal character array of this string.
         */
        std::array<char, MaxLen> data_{ 0 };

    } PACKED;
}