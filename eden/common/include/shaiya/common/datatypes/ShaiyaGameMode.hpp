#pragma once
#include <shaiya/common/util/Struct.hpp>

namespace shaiya
{
    /**
     * Represents the game mode values for Shaiya characters.
     */
    enum class ShaiyaGameMode : uint8_t
    {
        Easy,
        Normal,
        Hard,
        Ultimate
    };
}