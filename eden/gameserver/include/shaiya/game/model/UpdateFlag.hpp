#pragma once

namespace shaiya::game
{
    /**
     * Represents the various types of updates that may be signalled for an entity.
     */
    enum class UpdateFlag : uint32_t
    {
        Appearance    = (1u << 0u),
        Movement      = (1u << 1u),
        MovementState = (1u << 2u),
        Chat          = (1u << 3u),
        Yell          = (1u << 4u),
        MaxHitpoints  = (1u << 5u),
        Combat        = (1u << 6u),
    };
}