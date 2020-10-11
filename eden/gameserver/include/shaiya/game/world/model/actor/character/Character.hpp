#pragma once
#include <shaiya/common/net/packet/game/CharacterMovementState.hpp>
#include <shaiya/game/world/model/actor/Actor.hpp>
#include <shaiya/game/world/model/actor/character/ActionBar.hpp>
#include <shaiya/game/world/model/actor/character/Appearance.hpp>

#include <vector>

namespace shaiya::net
{
    class GameSession;  // Forward declaration of the session.
}

namespace shaiya::game
{
    class GameWorldService;

    /**
     * Represents a player character.
     */
    class Character: public Actor
    {
    public:
        /**
         * Creates a character instance from a connected session
         * @param session   The session instance.
         * @param id        The character id.
         */
        Character(shaiya::net::GameSession& session, size_t id);

        /**
         * Initialises this character.
         */
        void init() override;

        /**
         * Marks this character as active.
         */
        void activate() override;

        /**
         * Sets the movement state of a character.
         * @param movementState The new movement state.
         */
        void setMovementState(shaiya::net::MovementState movementState);

        /**
         * Resets the movement state of a character.
         */
        void resetMovementState();

        /**
         * Sets the amount of statpoints for this character.
         * @param statpoints    The character's statpoints.
         */
        void setStatpoints(size_t statpoints);

        /**
         * Sets the race for this character.
         * @param race  The race for this character.
         */
        void setRace(shaiya::ShaiyaRace race);

        /**
         * Gets the race of this character.
         * @return  The character's race.
         */
        [[nodiscard]] shaiya::ShaiyaRace race() const
        {
            return race_;
        }

        /**
         * Gets the movement state of this character.
         * @return  The movement state.
         */
        shaiya::net::MovementState movementState() const
        {
            return movementState_;
        }

        /**
         * Gets the action bar for this character.
         * @return  The character's action bar.
         */
        [[nodiscard]] ActionBar& actionBar()
        {
            return actionBar_;
        }

        /**
         * Gets the appearance of this character.
         * @return  The appearance.
         */
        [[nodiscard]] Appearance& appearance()
        {
            return appearance_;
        }

        /**
         * Gets the appearance of this character.
         * @return  The appearance.
         */
        [[nodiscard]] const Appearance& appearance() const
        {
            return appearance_;
        }

        /**
         * Gets the vector of entities that are in this character's viewport.
         * @return  The observed entities.
         */
        [[nodiscard]] std::vector<std::shared_ptr<Entity>>& observedEntities()
        {
            return observedEntities_;
        }

        /**
         * Gets the session for this character.
         * @return  The session.
         */
        [[nodiscard]] shaiya::net::GameSession& session() const
        {
            return session_;
        }

        /**
         * Gets the number of available stat points this character has.
         * @return  The total available stat points.
         */
        [[nodiscard]] uint32_t statpoints() const
        {
            return statpoints_;
        }

    private:
        /**
         * The game session instance.
         */
        shaiya::net::GameSession& session_;

        /**
         * The number of unused stat points the character has available to them.
         */
        uint16_t statpoints_{ 0 };

        /**
         * The number of unused skill points the character has available to them.
         */
        uint16_t skillpoints_{ 0 };

        /**
         * The number of kills the character has.
         */
        uint32_t kills_{ 0 };

        /**
         * The number of deaths the character has.
         */
        uint32_t deaths_{ 0 };

        /**
         * The number of victories the character has.
         */
        uint32_t victories_{ 0 };

        /**
         * The number of defeats the character has.
         */
        uint32_t defeats_{ 0 };

        /**
         * The movement state of this character.
         */
        shaiya::net::MovementState movementState_{ shaiya::net::MovementState::Standing };

        /**
         * The vector of entities that are in this character's viewport.
         */
        std::vector<std::shared_ptr<Entity>> observedEntities_;

        /**
         * This character's action bar.
         */
        ActionBar actionBar_;

        /**
         * The race of this character.
         */
        ShaiyaRace race_{ ShaiyaRace::Human };

        /**
         * The appearance of this character.
         */
        Appearance appearance_;

        /**
         * Gets executed when the stats for this character are synchronized.
         * @param stats     The stats for this character.
         */
        void onStatSync(const StatSet& stats, StatUpdateType type);
    };
}