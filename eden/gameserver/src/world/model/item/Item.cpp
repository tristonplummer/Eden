#include <shaiya/game/world/model/item/Item.hpp>

using namespace shaiya::game;

/**
 * Initialises an item by an item id.
 * @param id    The item id.
 */
Item::Item(uint32_t id): id_(id)
{
}

/**
 * Initialises an item by a type and type id.
 * @param type      The item type.
 * @param typeId    The type id.
 */
Item::Item(uint8_t type, uint8_t typeId): id_((type * 1000) + typeId)
{
}