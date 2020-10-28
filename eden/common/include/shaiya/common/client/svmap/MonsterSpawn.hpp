#pragma once

namespace shaiya::client
{
    /**
     * Represents a monster spawn.
     */
    struct MonsterSpawn
    {
        /**
         * The monster id.
         */
        uint32_t id{};

        /**
         * The number of monsters that are spawned in an area.
         */
        uint32_t count{};
    };
}