#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for the character details packet.
     */
    constexpr auto CharacterDetailsOpcode = 0x0105;

    /**
     * Informs the client of their character's details.
     */
    struct CharacterDetails
    {
        /**
         * The opcode of the character details packet.
         */
        uint16_t opcode{ CharacterDetailsOpcode };

        /**
         * The number of stat points that are available to the character.
         */
        uint16_t statpoints{ 0 };

        /**
         * The number of skill points that are available to the character.
         */
        uint16_t skillpoints{ 0 };

        /**
         * The maximum hitpoints of the character.
         */
        uint32_t maxHitpoints{ 0 };

        /**
         * The maximum mana of the character.
         */
        uint32_t maxMana{ 0 };

        /**
         * The maximum stamina of the character.
         */
        uint32_t maxStamina{ 0 };

        /**
         * The direction the character is facing.
         */
        uint16_t direction{ 0 };

        /**
         * The amount of experience required for the previous level.
         */
        uint32_t prevExp{ 0 };

        /**
         * The amount of experience required for the next level.
         */
        uint32_t nextExp{ 0 };

        /**
         * The current experience.
         */
        uint32_t currentExp{ 0 };

        /**
         * The amount of gold the character has in their inventory.
         */
        uint32_t gold{ 0 };

        /**
         * The x coordinate
         */
        float x{ 0.0f };

        /**
         * The y coordinate
         */
        float y{ 0.0f };

        /**
         * The z coordinate
         */
        float z{ 0.0f };

        /**
         * The number of kills the character has.
         */
        uint32_t kills{ 0 };

        /**
         * The number of deaths the character has.
         */
        uint32_t deaths{ 0 };

        /**
         * The number of victories the character has.
         */
        uint32_t victories{ 0 };

        /**
         * The number of defeats the character has.
         */
        uint32_t defeats{ 0 };

        /**
         * The guild name of the character.
         */
        VariableString<25> guildName;
    } PACKED;
}