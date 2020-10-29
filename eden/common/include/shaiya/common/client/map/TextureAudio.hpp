#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::client
{
    /**
     * Represents a pair of texture and audio names.
     */
    struct TextureAudio
    {
        /**
         * The name of the texture.
         */
        VariableString<256> texture;

        /**
         * Unknown. Sound channels, maybe?
         */
        float unknown{};

        /**
         * The name of the audio file.
         */
        VariableString<256> audio;
    } PACKED;
}