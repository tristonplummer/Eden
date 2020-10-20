#include <shaiya/game/world/model/item/Item.hpp>
#include <shaiya/game/world/model/item/container/ItemContainer.hpp>
#include <shaiya/game/world/model/item/container/event/ContainerEventListener.hpp>

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
 * Gets the item at a specific page and slot
 * @param page  The page
 * @param slot  The slot
 * @return      The item at the requested position
 */
std::shared_ptr<Item> ItemContainer::at(size_t page, size_t slot) const
{
    return at(pagePositionToIndex(page, slot));
}

/**
 * Gets the item at a specific slot
 * @param slot  The slot
 * @return      The item at the requested position
 */
std::shared_ptr<Item> ItemContainer::at(size_t slot) const
{
    if (slot >= items_.size())
        return nullptr;
    return items_.at(slot);
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

    items_.at(slot) = nullptr;
    for (auto&& listener: listeners_)
        listener->itemRemoved(*this, nullptr, slot);
    return item;
}

/**
 * Removes an item from the container at a specific slot.
 * @param page  The page.
 * @param slot  The slot.
 * @param count The amount of the item to remove.
 * @return      The item instance.
 */
std::shared_ptr<Item> ItemContainer::remove(size_t page, size_t slot, size_t count)
{
    auto idx = pagePositionToIndex(page, slot);
    if (idx >= items_.size())
        return nullptr;

    auto item = at(idx);
    if (count >= item->quantity())
        return remove(idx);

    item->setQuantity(item->quantity() - count);
    for (auto&& listener: listeners_)
        listener->itemRemoved(*this, item, idx);
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
    auto sourceItem = at(sourcePos);
    if (!sourceItem)
    {
        success = false;
        return { nullptr, nullptr };
    }
    items_.at(sourcePos) = nullptr;

    // The item at the current destination
    auto destItem = dest.at(destPos);

    // Add the item at the destination slot
    dest.items_.at(destPos) = sourceItem;

    // If there was an item at the destination slot, move it to the source slot
    if (destItem)
        items_.at(sourcePos) = destItem;
    success = true;

    for (auto&& listener: listeners_)
        listener->itemTransferred(*this, dest, sourcePos, destPos);
    for (auto&& listener: dest.listeners_)
        listener->itemTransferred(*this, dest, sourcePos, destPos);
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

/**
 * Sets the items of this container.
 * @param items The items.
 */
void ItemContainer::setItems(const std::vector<std::shared_ptr<Item>>& items)
{
    for (auto i = 0; i < items_.size(); i++)
    {
        items_[i] = items.at(i);
    }
}