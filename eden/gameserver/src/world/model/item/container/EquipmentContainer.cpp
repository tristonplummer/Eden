#include <shaiya/game/world/model/actor/character/Character.hpp>
#include <shaiya/game/world/model/item/container/event/EquipmentEventListener.hpp>

using namespace shaiya::game;

/**
 * The number of equipment slots.
 */
constexpr auto EquipmentCapacity = 8;

/**
 * Initialises this equipment container.
 * @param character The character that owns this equipment.
 */
EquipmentContainer::EquipmentContainer(Character& character): ItemContainer(EquipmentCapacity), character_(character)
{
    addListener(std::make_shared<EquipmentEventListener>(character));
}

/**
 * Adds an item to this container.
 * @param item  The item to add.
 * @return      If the item was successfully added.
 */
bool EquipmentContainer::add(std::shared_ptr<Item> item)
{
    auto result = ItemContainer::add(item);
    if (result)
    {
        character_.flagUpdate(UpdateMask::Appearance);
    }
    return result;
}

/**
 * Adds an item to this container at a specific slot.
 * @param item  The item to add.
 * @param slot  The requested slot.
 * @return      If the item was successfully added.
 */
bool EquipmentContainer::add(std::shared_ptr<Item> item, EquipmentSlot slot)
{
    auto result = ItemContainer::add(std::move(item), static_cast<uint8_t>(slot));
    if (result)
    {
        character_.flagUpdate(UpdateMask::Appearance);
    }
    return result;
}