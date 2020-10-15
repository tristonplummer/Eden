#pragma once
#include <shaiya/common/util/Struct.hpp>

namespace shaiya
{
    /**
     * Represents the class values of Shaiya characters.
     */
    enum class ShaiyaClass : uint8_t
    {
        Fighter,
        Defender,
        Ranger,
        Archer,
        Mage,
        Priest
    };
}