#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::client
{
    /**
     * Represents a Shaiya Model (SMod) that is placed in 3D space.
     */
    struct ShaiyaModelPosition
    {
        /**
         * The name id of the model. This is a number that corresponds to a previously loaded file name.
         */
        uint32_t id{};

        /**
         * The position in 3D space that this model is placed.
         */
        Shaiya3DVector position{};

        /**
         * The rotation matrix.
         */
        Shaiya3DVector rotationMatrix{};

        /**
         * The scaling matrix.
         */
        Shaiya3DVector scalingMatrix{};
    } PACKED;
}