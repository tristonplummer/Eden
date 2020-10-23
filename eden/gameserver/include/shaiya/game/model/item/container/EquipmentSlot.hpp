#pragma once

namespace shaiya::game
{
    /**
     * Represents a slot in the equipment container.
     */
    enum class EquipmentSlot : uint8_t
    {
        Helmet    = 0,
        Chest     = 1,
        Legs      = 2,
        Gloves    = 3,
        Boots     = 4,
        Weapon    = 5,
        Shield    = 6,
        Cape      = 7,
        Amulet    = 8,
        LeftRing  = 9,
        RightRing = 10,
        LeftLoop  = 11,
        RightLoop = 12,
        Mount     = 13,
        Costume   = 14,
        Pet       = 15,
        Wings     = 16,
    };
}