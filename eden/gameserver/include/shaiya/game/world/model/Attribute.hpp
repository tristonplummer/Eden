#pragma once
#include <string>

namespace shaiya::game
{
    /**
     * Represents a temporary attribute held by an entity.
     */
    enum class Attribute : uint32_t
    {
        /**
         * The last chat message sent by a character.
         */
        LastChatMessage,

        /**
         * The last request that was made by a character.
         */
        LastRequest,

        /**
         * The last character who requested something.
         */
        LastRequestingCharacter,

        /**
         * The currently active request instance.
         */
        Request
    };
}