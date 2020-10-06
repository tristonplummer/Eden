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
        LastChatMessage
    };
}