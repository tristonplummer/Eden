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
    for (auto i = 0; i < items_.size(); i++)
    {
        if (items_.at(i) == nullptr)
            return add(std::move(item), i);
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
    for (auto&& listener: listeners_)
        listener->itemAdded(*this, dest, slot);
    return true;
}

/**
 * Removes an item from the container at a specific slot.
 * @param slot  The slot.
 * @return      The item instance.
 */
std::shared_ptr<Item> ItemContainer::remove(size_t slot)
{
    auto item = items_.at(slot);
    if (!item)
        return nullptr;

    items_.erase(items_.begin() + slot);
    for (auto&& listener: listeners_)
        listener->itemRemoved(*this, item, slot);
    return item;
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
ItemPair ItemContainer::transferTo(ItemContainer& dest, size_t sourcePage, size_t sourceSlot, size_t destPage,
                                   size_t destSlot, bool& success)
{
    auto sourcePos = pagePositionToIndex(sourcePage, sourceSlot);
    auto destPos   = dest.pagePositionToIndex(destPage, destSlot);

    // Remove the item from the source
    auto sourceItem = remove(sourcePos);
    if (!sourceItem)
    {
        success = false;
        return { nullptr, nullptr };
    }

    // The item at the current destination
    auto destItem = dest.remove(destPos);

    // Add the item at the destination slot
    dest.add(sourceItem, destPos);

    // If there was an item at the destination slot, move it to the source slot
    if (destItem)
        add(destItem, sourcePos);
    success = true;
    return { sourceItem, destItem };
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