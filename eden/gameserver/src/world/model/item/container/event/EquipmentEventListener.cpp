#include <shaiya/common/net/packet/game/CharacterItem.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/world/model/actor/player/Player.hpp>
#include <shaiya/game/world/model/item/Item.hpp>
#include <shaiya/game/world/model/item/container/ItemContainer.hpp>
#include <shaiya/game/world/model/item/container/event/EquipmentEventListener.hpp>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Initialises this event listener for a character.
 * @param player The character that is listening to events.
 */
EquipmentEventListener::EquipmentEventListener(Player& player): player_(player)
{
}

/**
 * Gets executed when an item is added to a container.
 * @param container The container instance.
 * @param item      The item that was added.
 * @param slot      The slot of the new item.
 */
void EquipmentEventListener::itemAdded(const ItemContainer& container, const std::shared_ptr<Item>& item, size_t slot)
{
    player_.flagUpdate(UpdateFlag::Appearance);
    player_.syncStats();
}

/**
 * Gets executed when an item is removed from a container.
 * @param container The container instance.
 * @param item      The item that was removed.
 * @param slot      The slot the item was removed from.
 */
void EquipmentEventListener::itemRemoved(const ItemContainer& container, const std::shared_ptr<Item>& item, size_t slot)
{
    player_.flagUpdate(UpdateFlag::Appearance);
    player_.syncStats();
}

/**
 * Gets executed when an item is transferred between containers.
 * @param source        The source container.
 * @param dest          The destination container
 * @param sourceSlot    The source slot
 * @param destSlot      The destination slot.
 */
void EquipmentEventListener::itemTransferred(const ItemContainer& source, const ItemContainer& dest, size_t sourceSlot,
                                             size_t destSlot)
{
    player_.flagUpdate(UpdateFlag::Appearance);
    player_.syncStats();
}

/**
 * Synchronises the container with this listener.
 * @param container The container that was updated.
 */
void EquipmentEventListener::sync(const ItemContainer& container)
{
    auto& items = container.items();  // The items in the container
    CharacterItemUpdate update;       // Prepare the update
    update.quantity = items.size();

    // Iterate over the items
    for (auto i = 0; i < items.size(); i++)
    {
        auto& item = items.at(i);

        CharacterItemUnit unit;  // Prepare the unit
        unit.bag  = 0;           // Equipment is bag id 0.
        unit.slot = i;           // The slot of the item

        // If the item is valid, set the item details
        if (item)
        {
            unit.type     = item->type();
            unit.typeId   = item->typeId();
            unit.quantity = item->quantity();
        }

        update.items[i] = unit;
    }

    player_.session().write(update);  // Write the update
}