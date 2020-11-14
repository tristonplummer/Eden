#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * Repressents the status of an attack made on an actor.
     */
    enum class AttackStatus : uint8_t
    {
        Normal       = 0,
        Critical     = 1,
        Miss         = 2,
        Failed       = 3,
        CannotAttack = 10,
        WrongTarget  = 11,
    };
}