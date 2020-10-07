#pragma once
#include <shaiya/game/world/model/actor/character/Character.hpp>

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
         */
        virtual void load(Character& character) = 0;

        /**
         * Saves a player character.
         * @param character The character to save.
         */
        virtual void save(Character& character) = 0;
    };
}