#pragma once
#include <shaiya/common/util/Struct.hpp>

namespace shaiya
{
    /**
     * Represents a point in 3d space.
     */
    struct Shaiya3DVector
    {
        /**
         * The x-coordinate.
         */
        float x{};

        /**
         * The y-coordinate (vertical axis)
         */
        float y{};

        /**
         * The z-coordinate
         */
        float z{};
    } PACKED;
}