#include <shaiya/game/world/model/actor/character/Character.hpp>
#include <shaiya/game/world/model/item/container/event/InventoryEventListener.hpp>

using namespace shaiya::game;

/**
 * The number of pages in a character's inventory.
 */
constexpr auto InventoryPageCount = 5;

/**
 * The number of items that can fit on each page in the inventory.
 */
constexpr auto InventoryPageSize = 24;

/**
 * Initialises this inventory container.
 * @param character The character that owns this inventory.
 */
InventoryContainer::InventoryContainer(): ItemContainer(InventoryPageCount, InventoryPageSize)
{
}