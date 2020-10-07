#include <shaiya/common/net/packet/game/CharacterItem.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/world/model/item/container/event/EquipmentEventListener.hpp>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Initialises this event listener for a character.
 * @param character The character that is listening to events.
 */
EquipmentEventListener::EquipmentEventListener(Character& character): character_(character)
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
    character_.flagUpdate(UpdateFlag::Appearance);
    character_.syncStats();
}

/**
 * Gets executed when an item is removed from a container.
 * @param container The container instance.
 * @param item      The item that was removed.
 * @param slot      The slot the item was removed from.
 */
void EquipmentEventListener::itemRemoved(const ItemContainer& container, const std::shared_ptr<Item>& item, size_t slot)
{
    character_.flagUpdate(UpdateFlag::Appearance);
    character_.syncStats();
}

/**
 * Synchronises the container with this listener.
 * @param container The container that was updated.
 */
void EquipmentEventListener::sync(const ItemContainer& container)
{
    auto& items = container.items();  // The items in the container
    CharacterItemUpdate update;       // Prepare the update
    update.count = items.size();

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
            unit.type   = item->type();
            unit.typeId = item->typeId();
            unit.count  = item->count();
        }

        update.items[i] = unit;
    }

    character_.session().write(update);  // Write the update
}