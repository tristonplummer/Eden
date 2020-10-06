#pragma once
#include <shaiya/common/net/packet/game/CharacterMovementState.hpp>
#include <shaiya/game/world/model/actor/Actor.hpp>
#include <shaiya/game/world/model/actor/character/ActionBar.hpp>
#include <shaiya/game/world/model/item/container/EquipmentContainer.hpp>
#include <shaiya/game/world/model/item/container/InventoryContainer.hpp>

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
         * Sets the movement state of a character.
         * @param movementState The new movement state.
         */
        void setMovementState(shaiya::net::MovementState movementState);

        /**
         * Resets the movement state of a character.
         */
        void resetMovementState();

        /**
         * Gets the equipment for this character.
         * @return  The equipment.
         */
        [[nodiscard]] EquipmentContainer& equipment()
        {
            return equipment_;
        }

        /**
         * Gets the equipment for this character.
         * @return  The equipment.
         */
        [[nodiscard]] const EquipmentContainer& equipment() const
        {
            return equipment_;
        }

        /**
         * Gets the inventory for this character.
         * @return  The inventory.
         */
        [[nodiscard]] InventoryContainer& inventory()
        {
            return inventory_;
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
         * Gets the vector of characters that are in this character's viewport.
         * @return  The observed characters.
         */
        [[nodiscard]] std::vector<Character*>& observedCharacters()
        {
            return observedCharacters_;
        }

        /**
         * Gets the session for this character.
         * @return  The session.
         */
        [[nodiscard]] shaiya::net::GameSession& session() const
        {
            return session_;
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
         * The vector of characters that are in this character's viewport.
         */
        std::vector<Character*> observedCharacters_;

        /**
         * The worn equipment of the character.
         */
        EquipmentContainer equipment_;

        /**
         * The inventory of the character.
         */
        InventoryContainer inventory_;

        /**
         * This character's action bar.
         */
        ActionBar actionBar_;
    };
}