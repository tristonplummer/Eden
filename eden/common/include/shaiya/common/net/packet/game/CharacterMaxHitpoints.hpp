#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for informing a player about their max health, mana and stamina
     */
    constexpr auto MaxHitpointsOpcode = 0x050B;

    /**
     * The max hitpoints update type.
     */
    enum class MaxHitpointType: uint8_t
    {
        Hitpoints,
        Stamina,
        Mana
    };

    /**
     * Represents an update about a player's max hitpoints.
     */
    struct CharacterMaxHitpoints
    {
        /**
         * The opcode of the health update.
         */
        uint16_t opcode{ MaxHitpointsOpcode };

        /**
         * The character id that is being updated.
         */
        uint32_t id{ 0 };

        /**
         * The update type.
         */
        MaxHitpointType type{ MaxHitpointType::Hitpoints };

        /**
         * The value.
         */
        uint32_t value{ 0 };
    } PACKED;
}