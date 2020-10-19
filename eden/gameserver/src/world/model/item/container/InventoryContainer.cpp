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

/**
 * Initialises this inventory container.
 * @param inventory The inventory to copy from.
 */
InventoryContainer::InventoryContainer(const InventoryContainer& inventory)
    : ItemContainer(InventoryPageCount, InventoryPageSize)
{
    gold_ = inventory.gold();

    for (auto slot = 0; slot < items_.size(); slot++)
    {
        auto destItem = inventory.at(slot);
        if (destItem)
        {
            items_[slot] = destItem;
        }
    }
}

/**
 * Sets the amount of gold in the inventory.
 * @param gold  The amount of gold.
 */
void InventoryContainer::setGold(size_t gold)
{
    gold_ = gold;
}