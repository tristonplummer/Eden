#include <shaiya/game/world/model/item/Item.hpp>

using namespace shaiya::game;

/**
 * Initialises an item by it's definition.
 * @param id    The item definition
 */
Item::Item(const ItemDefinition& definition): definition_(definition)
{
}

/**
 * Set the item count.
 * @param count The number in this stack.
 */
void Item::setCount(size_t count)
{
    count_ = count;
}