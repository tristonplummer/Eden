#pragma once
#include <shaiya/game/Forward.hpp>

namespace shaiya::game
{
    /**
     * An interface that is designed to serialize and deserialize player characters.
     */
    class CharacterSerializer
    {
    public:
        /**
         * Loads a player character.
         * @param character The character to load.
         * @return          If the character was loaded successfully.
         */
        virtual bool load(Character& character) = 0;

        /**
         * Saves a player character.
         * @param character The character to save.
         */
        virtual void save(Character& character) = 0;
    };
}