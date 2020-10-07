#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode representing a character wanting to allocate their stats.
     */
    constexpr auto CharacterStatAllocationOpcode = 0x0208;

    /**
     * Represents a request for stat allocation.
     */
    struct CharacterStatAllocation
    {
        /**
         * The opcode of the stat allocation request.
         */
        uint16_t opcode{ CharacterStatAllocationOpcode };

        /**
         * The number of points to allocate into strength.
         */
        uint16_t strength{ 0 };

        /**
         * The number of points to allocate into dexterity.
         */
        uint16_t dexterity{ 0 };

        /**
         * The number of points to allocate into reaction.
         */
        uint16_t reaction{ 0 };

        /**
         * The number of points to allocate into intelligence.
         */
        uint16_t intelligence{ 0 };

        /**
         * The number of points to allocate into wisdom.
         */
        uint16_t wisdom{ 0 };

        /**
         * The number of points to allocate into luck.
         */
        uint16_t luck{ 0 };
    } PACKED;
}