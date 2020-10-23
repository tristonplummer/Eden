#include <shaiya/common/net/packet/game/CharacterAddItem.hpp>
#include <shaiya/common/net/packet/game/CharacterItem.hpp>
#include <shaiya/common/net/packet/game/CharacterRemoveItem.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/world/model/actor/player/Player.hpp>
#include <shaiya/game/world/model/item/Item.hpp>
#include <shaiya/game/world/model/item/container/event/InventoryEventListener.hpp>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Initialises this event listener for a character.
 * @param player The character that is listening to events.
 */
InventoryEventListener::InventoryEventListener(Player& player): player_(player)
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
    // Prepare the added item request
    CharacterAddItem update;

    // Calculate the page and slot
    update.page     = (slot / container.pageSize()) + 1;
    update.slot     = slot % container.pageSize();
    update.type     = item ? item->type() : 0;
    update.typeId   = item ? item->typeId() : 0;
    update.quantity = item ? item->quantity() : 0;
    player_.session().write(update);
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
    update.page     = (slot / container.pageSize()) + 1;
    update.slot     = slot % container.pageSize();
    update.type     = item ? item->type() : 0;
    update.typeId   = item ? item->typeId() : 0;
    update.quantity = item ? item->quantity() : 0;
    player_.session().write(update);
}

/**
 * Synchronises the container with this listener.
 * @param container The container that was updated.
 */
void InventoryEventListener::sync(const ItemContainer& container)
{
    auto& session = player_.session();
    auto& items   = container.items();  // The items in the container
    CharacterItemUpdate update;         // The item update

    // Loop over the items in the container - we want to chunk the update into sets of 50.
    for (auto i = 0; i < items.size(); i++)
    {
        if (update.quantity == CharacterItemUpdateCapacity)  // Write this chunk
        {
            session.write(update);
            update = {};
        }

        // The item at the current slot
        auto& item = items.at(i);
        if (!item)
            continue;

        // The item slot to update
        auto& itemUpdate = update.items[update.quantity];
        update.quantity++;

        // Calculate the page and slot
        itemUpdate.bag  = (i / container.pageSize()) + 1;
        itemUpdate.slot = i % container.pageSize();

        // Write the item details
        itemUpdate.type       = item->type();
        itemUpdate.typeId     = item->typeId();
        itemUpdate.quantity   = item->quantity();
        itemUpdate.durability = item->durability();
    }

    // Write the left over items
    session.write(update, 3 + (update.quantity * sizeof(CharacterItemUnit)));
}