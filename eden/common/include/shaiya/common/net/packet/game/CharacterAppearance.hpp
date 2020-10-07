#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/net/packet/game/CharacterMovementState.hpp>

#include <array>

namespace shaiya::net
{
    /**
     * The opcode for updating a character of another character's appearance.
     */
    constexpr auto CharacterAppearanceOpcode = 0x0303;

    /**
     * The number of equipment slots that are visible.
     */
    constexpr auto VisibleEquipmentLength = 16;

    /**
     * Represents the appearance of an item slot.
     */
    struct ItemAppearance
    {
        /**
         * The item type.
         */
        uint8_t type{ 0 };

        /**
         * The item type id.
         */
        uint8_t typeId{ 0 };

        /**
         * The enchant value.
         */
        uint8_t enchant{ 0 };
    } PACKED;

    /**
     * Represents a character appearance update.
     */
    struct CharacterAppearance
    {
        /**
         * The opcode of the character appearance update.
         */
        uint16_t opcode{ CharacterAppearanceOpcode };

        /**
         * The id of the character.
         */
        uint32_t id{ 0 };

        /**
         * If the character is alive.
         */
        NexonBoolean alive{ true };

        /**
         * The movement state of the character.
         */
        MovementState state{ MovementState::Standing };

        /**
         * The faction of the character.
         */
        ShaiyaFaction faction{ ShaiyaFaction::Neither };

        /**
         * The race of the character.
         */
        ShaiyaRace race{ ShaiyaRace::Human };

        /**
         * The hair of the character.
         */
        uint8_t hair{ 0 };

        /**
         * The face of the character.
         */
        uint8_t face{ 0 };

        /**
         * The height of the character.
         */
        uint8_t height{ 0 };

        /**
         * The class of the character.
         */
        ShaiyaClass job{ ShaiyaClass::Fighter };

        /**
         * The gender of the character.
         */
        ShaiyaGender gender{ ShaiyaGender::Male };

        /**
         * If the character is in a party.
         */
        bool party{ false };

        /**
         * The game mode of the character.
         */
        ShaiyaGameMode mode{ ShaiyaGameMode::Ultimate };

        /**
         * The amount of kills the character has.
         */
        uint32_t kills{ 0 };

        /**
         * The visible equipment of the character.
         */
        std::array<ItemAppearance, VisibleEquipmentLength> equipment;

        /**
         * Defines whether each equipment slot is dyed.
         */
        std::array<bool, VisibleEquipmentLength> equipmentDyeFlags{ false };

        /**
         * TODO: Refactor this.
         */
        uint32_t unknown{ 0 };

        /**
         * The dye colours for each piece of equipment
         */
        std::array<uint32_t, VisibleEquipmentLength> equipmentDyes { 0 };

        /**
         * TODO: Refactor this, also
         */
        VariableString<459> unk;

        /**
         * The name of the character.
         */
        VariableString<21> name;

        /**
         * The name of the guild that the character is in.
         */
        VariableString<31> guild;
    } PACKED;
}