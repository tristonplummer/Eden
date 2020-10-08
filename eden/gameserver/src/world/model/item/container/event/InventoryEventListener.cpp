#include <shaiya/common/net/packet/game/CharacterItem.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/world/model/item/container/event/InventoryEventListener.hpp>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Initialises this event listener for a character.
 * @param character The character that is listening to events.
 */
InventoryEventListener::InventoryEventListener(Character& character): character_(character)
{
}

/**
 * Gets executed when an item is added to a container.
 * @param container The container instance.
 * @param item      The item that was added.
 * @param slot      The slot of the new item.
 */
void InventoryEventListener::itemAdded(const ItemContainer& container, const std::shared_ptr<Item>& item, size_t slot)
{
}

/**
 * Gets executed when an item is removed from a container.
 * @param container The container instance.
 * @param item      The item that was removed.
 * @param slot      The slot the item was removed from.
 */
void InventoryEventListener::itemRemoved(const ItemContainer& container, const std::shared_ptr<Item>& item, size_t slot)
{
    // Prepare the removed item request
    CharacterRemoveItem update;

    // Calculate the page and slot
    update.page   = (slot / container.pageSize()) + 1;
    update.slot   = slot % container.pageSize();
    update.type   = item ? item->type() : 0;
    update.typeId = item ? item->typeId() : 0;
    update.count  = item ? item->count() : 0;
    character_.session().write(update);
}

/**
 * Synchronises the container with this listener.
 * @param container The container that was updated.
 */
void InventoryEventListener::sync(const ItemContainer& container)
{
    auto& session = character_.session();
    auto& items   = container.items();  // The items in the container
    CharacterItemUpdate update;         // The item update

    // Loop over the items in the container - we want to chunk the update into sets of 50.
    for (auto i = 0; i < items.size(); i++)
    {
        if (update.count == CharacterItemUpdateCapacity)  // Write this chunk
        {
            session.write(update);
            update = {};
        }

        // The item at the current slot
        auto& item = items.at(i);
        if (!item)
            continue;

        // The item slot to update
        auto& itemUpdate = update.items[update.count];
        update.count++;

        // Calculate the page and slot
        itemUpdate.bag  = (i / container.pageSize()) + 1;
        itemUpdate.slot = i % container.pageSize();

        // Write the item details
        itemUpdate.type       = item->type();
        itemUpdate.typeId     = item->typeId();
        itemUpdate.count      = item->count();
        itemUpdate.durability = item->durability();
    }

    // Write the left over items
    session.write(update, 3 + (update.count * sizeof(CharacterItemUnit)));
}