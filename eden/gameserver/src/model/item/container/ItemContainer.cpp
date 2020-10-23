#include <shaiya/game/model/item/Item.hpp>
#include <shaiya/game/model/item/container/ItemContainer.hpp>
#include <shaiya/game/model/item/container/event/ContainerEventListener.hpp>

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
    if (!item)
        return false;

    auto emptySlot = -1;
    for (auto i = 0; i < items_.size(); i++)
    {
        if (item->quantity() == 0)
            return true;

        auto& dest = items_.at(i);
        if (!dest)
        {
            if (emptySlot == -1)
                emptySlot = i;
            continue;
        }

        if (dest->itemId() == item->itemId())
        {
            auto& def      = item->definition();
            auto freeSpace = def.maxStack - dest->quantity();
            auto quantity  = std::min(freeSpace, item->quantity());

            if (freeSpace > 0)
            {
                dest->setQuantity(dest->quantity() + quantity);
                item->setQuantity(item->quantity() - quantity);

                for (auto&& listener: listeners_)
                    listener->itemAdded(*this, dest, i);
            }
            continue;
        }
    }

    if (emptySlot != -1)
        return add(std::move(item), emptySlot);
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
 * @param slot      The slot.
 * @return          The item instance.
 */
std::shared_ptr<Item> ItemContainer::remove(size_t slot)
{
    if (slot >= items_.size())
        return nullptr;

    auto item = items_.at(slot);
    if (!item)
        return nullptr;
    return remove(slot, item->quantity());
}

/**
 * Removes an item from the container at a specific slot.
 * @param slot      The slot.
 * @param quantity  The quantity to remove.
 * @return          The item instance.
 */
std::shared_ptr<Item> ItemContainer::remove(size_t slot, size_t quantity)
{
    if (slot >= items_.size())
        return nullptr;

    auto item = items_.at(slot);
    if (!item)
        return nullptr;

    if (quantity >= item->quantity())
    {
        items_.at(slot) = nullptr;

        for (auto&& listener: listeners_)
            listener->itemRemoved(*this, nullptr, slot);
    }
    else
    {
        item->setQuantity(item->quantity() - quantity);

        if (item->quantity() >= 0)
        {
            for (auto&& listener: listeners_)
                listener->itemRemoved(*this, item, slot);
        }
        else
        {
            items_.at(slot) = nullptr;
            for (auto&& listener: listeners_)
                listener->itemRemoved(*this, nullptr, slot);
        }
    }

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
    return remove(pagePositionToIndex(page, slot), count);
}

/**
 * Transfers an item from this container to another.
 * @param dest          The destination container.
 * @param sourceSlot    The source slot.
 * @param destSlot      The destination slot.
 * @param success       If the transfer was successful.
 * @return              The item at the source position, and the item at the destination position.
 */
ItemPair ItemContainer::transferTo(ItemContainer& dest, size_t sourceSlot, size_t destSlot, bool& success)
{
    auto sourceItem = at(sourceSlot);
    auto sourceQty  = sourceItem ? sourceItem->quantity() : 1;

    return transferTo(dest, sourceSlot, sourceQty, destSlot, success);
}

/**
 * Transfers an item from this container to another.
 * @param dest          The destination container.
 * @param sourceSlot    The source slot.
 * @param sourceQty     The quantity of the source item to transfer.
 * @param destSlot      The destination slot.
 * @param success       If the transfer was successful.
 * @return              The item at the source position, and the item at the destination position.
 */
ItemPair ItemContainer::transferTo(ItemContainer& dest, size_t sourceSlot, size_t sourceQty, size_t destSlot, bool& success)
{
    // The items at the specified positions
    auto sourceItem = at(sourceSlot);
    auto destItem   = dest.at(destSlot);

    // A helper function to fail a transfer
    auto fail = [&]() -> ItemPair {
        success = false;
        return { nullptr, nullptr };
    };

    // A helper function for successfully executing a transfer
    auto ok = [&]() -> ItemPair {
        for (auto&& listener: listeners_)
            listener->itemTransferred(*this, dest, sourceSlot, destSlot);
        for (auto&& listener: dest.listeners_)
            listener->itemTransferred(*this, dest, sourceSlot, destSlot);

        success = true;
        return { sourceItem, destItem };
    };

    // If we're trying to move nothing, just fail early.
    if (!sourceItem)
        return fail();

    // Adjust the source quantity
    sourceQty = std::min(sourceItem->quantity(), sourceQty);

    // If an item exists in the destination slot
    if (destItem)
    {
        // Attempt to merge the source stack to the destination
        if (sourceItem->itemId() == destItem->itemId())
        {
            auto& def      = destItem->definition();
            auto freeSpace = def.maxStack - destItem->quantity();
            auto quantity  = std::min(freeSpace, sourceItem->quantity());

            destItem->setQuantity(destItem->quantity() + quantity);
            sourceItem->setQuantity(sourceItem->quantity() - quantity);
            return ok();
        }

        // If we're trying to move a partial stack into an occupied slot, fail
        if (sourceItem->quantity() != sourceQty)
            return fail();

        // Swap the items
        items_.at(sourceSlot)    = destItem;
        dest.items_.at(destSlot) = sourceItem;
        return ok();
    }

    // If we're trying to move a partial stack, then we need to create a copy of the item and adjust the quantities
    if (sourceItem->quantity() != sourceQty)
    {
        // Remove the quantity from the source item
        sourceItem->setQuantity(sourceItem->quantity() - sourceQty);

        // Create a new item with the split quantity
        destItem = std::make_shared<Item>(*sourceItem);
        destItem->setQuantity(sourceQty);

        // Move the new item into the destination
        dest.items_.at(destSlot) = destItem;
        return ok();
    }

    // Swap the two items
    items_.at(sourceSlot)    = destItem;
    dest.items_.at(destSlot) = sourceItem;
    return ok();
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
 * Removes a listener from this container.
 * @param listener  The listener to remove.
 */
void ItemContainer::removeListener(const std::shared_ptr<ContainerEventListener>& listener)
{
    auto pos = std::find(listeners_.begin(), listeners_.end(), listener);
    if (pos != listeners_.end())
        listeners_.erase(pos);
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