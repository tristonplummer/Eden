#pragma once
#include <shaiya/game/world/model/item/container/ContainerEventListener.hpp>

namespace shaiya::game
{
    class Character;

    /**
     * A specialised container event listener, that listens for events on a character's equipment.
     */
    class EquipmentEventListener: public ContainerEventListener
    {
    public:
        /**
         * Initialises this event listener for a character.
         * @param character The character that is listening to events.
         */
        EquipmentEventListener(Character& character);

        /**
         * Synchronises the container with this listener.
         * @param container The container that was updated.
         */
        void sync(const ItemContainer& container) override;

    private:
        /**
         * The character that is listening for these updates.
         */
        Character& character_;
    };
}