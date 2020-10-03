#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for the character creation packet.
     */
    constexpr auto CharacterCreateOpcode = 0x0102;

    /**
     * The result of a character creation operation.
     */
    enum class CharacterCreateResult : uint8_t
    {
        Success,
        NameNotAvailable,
        Error
    };

    /**
     * A packet that informs the player's account of their account's faction.
     */
    struct CharacterCreationRequest
    {
        /**
         * The opcode for the character creation.
         */
        uint16_t opcode{ CharacterCreateOpcode };

        /**
         * The destination slot in the character list
         */
        uint8_t slot{ 0 };

        /**
         * The character's race.
         */
        ShaiyaRace race{ ShaiyaRace::Human };

        /**
         * The game mode of the character.
         */
        ShaiyaGameMode mode{ ShaiyaGameMode::Easy };

        /**
         * The hair style value of the character
         */
        uint8_t hair{ 0 };

        /**
         * The face value of the character
         */
        uint8_t face{ 0 };

        /**
         * The height of the character
         */
        uint8_t height{ 0 };

        /**
         * The class of the character. We use the name 'job' as that's how it is referred to in the
         * Ep4 files, and because 'class' is a reserved keyword.
         */
        ShaiyaClass job{ ShaiyaClass::Fighter };

        /**
         * The gender of the character
         */
        ShaiyaGender gender{ ShaiyaGender::Male };

        /**
         * The name of the character
         */
        VariableString<21> name;
    } PACKED;

    /**
     * The response of a character creation request.
     */
    struct CharacterCreationResponse
    {
        /**
         * The opcode for the character creation.
         */
        uint16_t opcode{ CharacterCreateOpcode };

        /**
         * The result of the character creation request.
         */
        CharacterCreateResult result{ CharacterCreateResult::Error };
    } PACKED;
}