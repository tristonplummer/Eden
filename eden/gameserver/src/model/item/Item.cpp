#include <shaiya/game/model/item/Item.hpp>

using namespace shaiya::game;

/**
 * Initialises an item by it's definition.
 * @param id    The item definition
 */
Item::Item(const shaiya::client::ItemDefinition& definition): definition_(definition)
{
}

/**
 * Copy constructor.
 * @param item  The item to copy from.
 */
Item::Item(const Item& item): definition_(item.definition())
{
    quantity_ = item.quantity();
}

/**
 * Set the item quantity.
 * @param quantity The number in this stack.
 */
void Item::setQuantity(size_t quantity)
{
    quantity_ = quantity;
}