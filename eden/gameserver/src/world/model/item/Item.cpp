#include <shaiya/game/world/model/item/Item.hpp>

using namespace shaiya::game;

/**
 * Initialises an item by it's definition.
 * @param id    The item definition
 */
Item::Item(const shaiya::client::ItemDefinition& definition): definition_(definition)
{
}

/**
 * Set the item quantity.
 * @param quantity The number in this stack.
 */
void Item::setQuantity(size_t quantity)
{
    quantity_ = quantity;
}