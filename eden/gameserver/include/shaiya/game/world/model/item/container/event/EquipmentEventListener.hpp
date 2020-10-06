#pragma once
#include <shaiya/game/world/model/item/container/event/ContainerEventListener.hpp>

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
         * Gets executed when an item is added to a container.
         * @param container The container instance.
         * @param item      The item that was added.
         * @param slot      The slot of the new item.
         */
        void itemAdded(const ItemContainer& container, const std::shared_ptr<Item>& item, size_t slot) override;

        /**
         * Gets executed when an item is removed from a container.
         * @param container The container instance.
         * @param item      The item that was removed.
         * @param slot      The slot the item was removed from.
         */
        void itemRemoved(const ItemContainer& container, const std::shared_ptr<Item>& item, size_t slot) override;

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