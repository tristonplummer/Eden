#pragma once
#include <shaiya/game/world/model/item/Item.hpp>
#include <shaiya/game/world/model/item/container/ContainerEventListener.hpp>

#include <memory>
#include <vector>

namespace shaiya::game
{
    /**
     * A container of items in the world.
     */
    class ItemContainer
    {
    public:
        /**
         * Initialises a multi page container, with each page having a set capacity.
         * @param pages     The number of pages.
         * @param pageSize  The capacity of a single page.
         */
        ItemContainer(size_t pages, size_t pageSize);

        /**
         * Initialises a single page container with a specified capacity.
         * @param capacity  The capacity.
         */
        explicit ItemContainer(size_t capacity);

        /**
         * Adds an item to this container.
         * @param item  The item to add.
         * @return      If the item was successfully added.
         */
        virtual bool add(std::shared_ptr<Item> item);

        /**
         * Adds an item to this container at a specific slot.
         * @param item  The item to add.
         * @param slot  The requested slot.
         * @return      If the item was successfully added.
         */
        virtual bool add(std::shared_ptr<Item> item, size_t slot);

        /**
         * Adds a listener to this container.
         * @param listener  The listener.
         */
        void addListener(std::shared_ptr<ContainerEventListener> listener);

        /**
         * Synchronises this container for all listeners.
         */
        virtual void sync();

        /**
         * Get the vector of items in this container.
         * @return  The vector of items.
         */
        [[nodiscard]] const std::vector<std::shared_ptr<Item>>& items() const
        {
            return items_;
        }

    private:
        /**
         * The vector of items held by this container.
         */
        std::vector<std::shared_ptr<Item>> items_;

        /**
         * The vector of listeners.
         */
        std::vector<std::shared_ptr<ContainerEventListener>> listeners_;

        /**
         * The number of virtual "pages" in this container.
         */
        size_t pageCount_{ 0 };

        /**
         * The size of each page.
         */
        size_t pageSize_{ 0 };
    };
}