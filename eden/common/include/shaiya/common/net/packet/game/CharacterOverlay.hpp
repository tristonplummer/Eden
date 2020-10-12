#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for display an overlay on the character.
     */
    constexpr auto CharacterOverlayOpcode = 0x0240;

    /**
     * The flags for specifying which effects should be interpreted.
     */
    enum OverlayFlag : uint8_t
    {
        Colour  = (1u << 1u),
        Title   = (1u << 2u),
        Overlay = (1u << 3u)
    };

    /**
     * Represents an update about a character's overlay.
     */
    struct CharacterOverlay
    {
        /**
         * The opcode for this overlay uppdate.
         */
        uint16_t opcode{ CharacterOverlayOpcode };

        /**
         * The id of the character we are updating.
         */
        uint32_t id{ 0 };

        /**
         * If the overlay is visible.
         */
        bool visible{ true };

        /**
         * The overlay mask, which is a value that represents the types of overlays to apply, calculating as such:
         * overlayMask = OverlayFlag::Title | OverlayFlag::Colour
         */
        uint8_t overlayMask{ 0 };

        /**
         * The primary colour of the character's name.
         */
        uint32_t primaryColour{ 0 };

        /**
         * The secondary colour of the character's name. If this is different from the primary colour, the character's name
         * will flicker between these two colours.
         */
        uint32_t secondaryColour{ 0 };

        /**
         * The effect id to show over this character.
         */
        uint32_t overlayEffect{ 0 };

        /**
         * The effect id to show under this character.
         */
        uint32_t underlayEffect{ 0 };

        /**
         * The title to display above the character's guild name.
         */
        VariableString<32> title;
    } PACKED;
}