#pragma once
#include <shaiya/game/world/model/item/Item.hpp>
#include <shaiya/game/world/model/item/container/event/ContainerEventListener.hpp>

#include <memory>
#include <tuple>
#include <vector>

namespace shaiya::game
{
    typedef std::tuple<std::shared_ptr<Item>, std::shared_ptr<Item>> ItemPair;  // A pair of items, represented as a tuple.

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
         * Removes an item from the container at a specific slot.
         * @param slot  The slot.
         * @return      The item instance.
         */
        virtual std::shared_ptr<Item> remove(size_t slot);

        /**
         * Gets the position in a page as a local index.
         * @param page  The page.
         * @param slot  The slot.
         * @return      The index.
         */
        [[nodiscard]] size_t pagePositionToIndex(size_t page, size_t slot) const
        {
            return (page * pageSize_) + slot;
        }

        /**
         * Transfers an item from this container to another.
         * @param dest          The destination container.
         * @param sourcePage    The source page.
         * @param sourceSlot    The source slot.
         * @param destPage      The destination page.
         * @param destSlot      The destination slot.
         * @param success       If the transfer was successful.
         * @return              The item at the source position, and the item at the destination position.
         */
        ItemPair transferTo(ItemContainer& dest, size_t sourcePage, size_t sourceSlot, size_t destPage,
                                       size_t destSlot, bool& success);

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
         * Gets the number of pages in this container.
         * @return  The page count.
         */
        [[nodiscard]] size_t pageCount() const
        {
            return pageCount_;
        }

        /**
         * Gets the size of each page in this container.
         * @return  The page size.
         */
        [[nodiscard]] size_t pageSize() const
        {
            return pageSize_;
        }

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