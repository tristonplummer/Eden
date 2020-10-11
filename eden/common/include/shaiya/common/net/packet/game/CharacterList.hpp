#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for displaying the account's list of available characters.
     */
    constexpr auto CharacterListOpcode = 0x0101;

    /**
     * The character list is actually multiple "character entry" packets.
     * Each character entry packet specifies the slot in the character list, the
     * character id (or 0 if the slot is empty), and some basic details about the character.
     */
    struct CharacterListEntry
    {
        /**
         * The opcode for the character list.
         */
        uint16_t opcode{ CharacterListOpcode };

        /**
         * The slot in the character list
         */
        uint8_t slot{ 0 };

        /**
         * The character id
         */
        uint32_t id{ 0 };

        /**
         * The timestamp that the character was created. Honestly I'm not sure why the client
         * would / should ever need to know this, so let's just keep it as 0.
         */
        uint32_t creationTime{ 0 };

        /**
         * The level of the character
         */
        uint16_t level{ 0 };

        /**
         * The character's race.
         */
        ShaiyaRace race{ ShaiyaRace::Human };

        /**
         * The game mode of the character.
         * 0 = Easy
         * 1 = Normal
         * 2 = Hard
         * 3 = Ultimate
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
         * 0 = Male
         * 1 = Female
         */
        uint8_t gender{ 0 };

        /**
         * The map id that the character is currently located at
         */
        uint16_t map{ 0 };

        /**
         * The strength stat
         */
        uint16_t strength{ 0 };

        /**
         * The dexterity stat
         */
        uint16_t dexterity{ 0 };

        /**
         * The reaction (or "REC") stat. I don't like this name, but it's how
         * it's always been referred to in Shaiya. I feel like this should just be called
         * "resistance", and we should make the distinction between physical and magical resistance.
         *
         * For sake of consistency, we will continue to refer to it as "reaction".
         */
        uint16_t reaction{ 0 };

        /**
         * The intelligence stat
         */
        uint16_t intelligence{ 0 };

        /**
         * The wisdom stat
         */
        uint16_t wisdom{ 0 };

        /**
         * The luck stat
         */
        uint16_t luck{ 0 };

        /**
         * The current hitpoints of the character
         */
        uint16_t hitpoints{ 0 };

        /**
         * The current mana of the character
         */
        uint16_t mana{ 0 };

        /**
         * The current stamina of the character
         */
        uint16_t stamina{ 0 };

        /**
         * The item "types" for the character's worn equipment
         */
        std::array<char, 17> equipmentTypes{ 0 };

        /**
         * The item "type ids" for the character's worn equipment
         */
        std::array<char, 17> equipmentTypeIds{ 0 };

        /**
         * Flags that represent if an equipment slot has custom dye.
         */
        std::array<bool, 17> equipmentDyeFlags{ false };

        /**
         * The data of the dyed items.
         */
        VariableString<523> dyeData;

        /**
         * The name of the character
         */
        VariableString<19> name;

        /**
         * If this character is pending to be deleted.
         */
        bool pendingDeletion{ false };

        /**
         * The cloak data (emblems, colours, etc)
         */
        std::array<char, 6> cloakData{ 0 };
    } PACKED;
}