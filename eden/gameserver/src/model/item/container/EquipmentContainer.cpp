#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/item/container/event/EquipmentEventListener.hpp>

using namespace shaiya::game;

/**
 * The number of equipment slots.
 */
constexpr auto EquipmentCapacity = 17;

/**
 * Initialises this equipment container.
 */
EquipmentContainer::EquipmentContainer(): ItemContainer(EquipmentCapacity)
{
}

/**
 * Gets the item at a specific page and slot
 * @param page  The page
 * @param slot  The slot
 * @return      The item at the requested position
 */
std::shared_ptr<Item> EquipmentContainer::at(EquipmentSlot slot) const
{
    return ItemContainer::at(static_cast<size_t>(slot));
}

/**
 * Adds an item to this container.
 * @param item  The item to add.
 * @return      If the item was successfully added.
 */
bool EquipmentContainer::add(std::shared_ptr<Item> item)
{
    return ItemContainer::add(item);
}

/**
 * Adds an item to this container at a specific slot.
 * @param item  The item to add.
 * @param slot  The requested slot.
 * @return      If the item was successfully added.
 */
bool EquipmentContainer::add(std::shared_ptr<Item> item, EquipmentSlot slot)
{
    return ItemContainer::add(std::move(item), static_cast<uint8_t>(slot));
}

/**
 * Gets the vector of items that are visible by other characters.
 * @return  The visible items
 */
std::vector<std::shared_ptr<Item>> EquipmentContainer::visibleItems() const
{
    auto lastVisibleSlot = static_cast<uint8_t>(EquipmentSlot::Wings);
    return std::vector<std::shared_ptr<Item>>(items().begin(), items().begin() + lastVisibleSlot + 1);
}