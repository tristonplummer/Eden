#include <shaiya/game/world/model/item/container/ItemContainer.hpp>

using namespace shaiya::game;

/**
 * Initialises a single page container with a specified capacity.
 * @param capacity  The capacity.
 */
ItemContainer::ItemContainer(size_t capacity): ItemContainer(1, capacity)
{
}

/**
 * Initialises a multi page container, with each page having a set capacity.
 * @param pages     The number of pages.
 * @param pageSize  The capacity of a single page.
 */
ItemContainer::ItemContainer(size_t pages, size_t pageSize)
{
    pageCount_ = pages;
    pageSize_  = pageSize;
    items_.resize(pageCount_ * pageSize_);
}

/**
 * Adds an item to this container.
 * @param item  The item to add.
 * @return      If the item was successfully added.
 */
bool ItemContainer::add(std::shared_ptr<Item> item)
{
    for (auto&& dest: items_)  // Loop through the container items
    {
        if (!dest)  // If no item exists at this point
        {
            dest = std::move(item);
            return true;
        }
    }
    return false;
}

/**
 * Adds an item to this container at a specific slot.
 * @param item  The item to add.
 * @param slot  The requested slot.
 * @return      If the item was successfully added.
 */
bool ItemContainer::add(std::shared_ptr<Item> item, size_t slot)
{
    auto& dest = items_.at(slot);
    if (dest)
        return false;
    dest = std::move(item);
    return true;
}

/**
 * Adds a listener to this container.
 * @param listener  The listener.
 */
void ItemContainer::addListener(std::shared_ptr<ContainerEventListener> listener)
{
    listeners_.push_back(std::move(listener));
}

/**
 * Synchronises this container for all listeners.
 */
void ItemContainer::sync()
{
    for (auto&& listener: listeners_)
        listener->sync(*this);
}