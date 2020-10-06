#include <shaiya/common/net/packet/game/CharacterItem.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/world/model/item/container/EquipmentEventListener.hpp>

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
            unit.count  = 1;
        }

        update.items[i] = unit;
    }

    character_.session().write(update);  // Write the update
}