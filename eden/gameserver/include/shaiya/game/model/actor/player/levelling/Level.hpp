#pragma once
#include <shaiya/game/Forward.hpp>

namespace shaiya::game
{
    /**
     * Represents the level data of a player.
     */
    class Level
    {
    public:
        /**
         * Initialises the level of this player.
         * @param player    The player.
         */
        explicit Level(Player& player);

        /**
         * Initialises the level data for a player.
         * @param mode          The game mode of the player.
         * @param level         The player's current level.
         * @param experience    The player's current experience.
         */
        void init(ShaiyaGameMode mode, uint16_t level, uint16_t experience);

        /**
         * Sets the experience of a player.
         * @param experience    The new experience value.
         */
        void setExperience(size_t experience);

        /**
         * Adds experience to the player.
         * @param experience    The experience to add.
         */
        void addExperience(size_t experience);

        /**
         * Sets the level of the player.
         * @param level             The new level.
         * @param adjustExperience  Adjust the player's experience to match the new level.
         */
        void setLevel(uint16_t level, bool adjustExperience = false);

        /**
         * Gets the level of this actor.
         * @return  The level of the actor.
         */
        [[nodiscard]] uint16_t level() const
        {
            return level_;
        }

        /**
         * Gets the base experience of the level.
         * @return  The base experience required to reach the current level.
         */
        [[nodiscard]] uint32_t baseExperience() const
        {
            return baseExperience_;
        }

        /**
         * Gets the current experience of the player.
         * @return  The current experience.
         */
        [[nodiscard]] uint32_t experience() const
        {
            return experience_;
        }

        /**
         * Gets the experience required to level up.
         * @return  The required experience.
         */
        [[nodiscard]] uint32_t requiredExperience() const
        {
            return requiredExperience_;
        }

    private:
        /**
         * The player instance.
         */
        Player& player_;

        /**
         * The world service.
         */
        GameWorldService& world_;

        /**
         * The game mode of the player.
         */
        ShaiyaGameMode mode_{};

        /**
         * The current level of the player.
         */
        uint16_t level_{};

        /**
         * The max level available to the player.
         */
        uint16_t maxLevel_{};

        /**
         * The base experience required to reach the current level.
         */
        size_t baseExperience_{};

        /**
         * The current experience of the player.
         */
        size_t experience_{};

        /**
         * The required experience to complete the level.
         */
        size_t requiredExperience_{};
    };
}