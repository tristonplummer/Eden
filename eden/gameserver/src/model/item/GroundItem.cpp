#include <shaiya/game/model/item/GroundItem.hpp>

using namespace shaiya::game;

/**
 * Creates an item that is to be dropped on the ground.
 * @param item  The item instance.
 */
GroundItem::GroundItem(std::shared_ptr<Item> item, GameWorldService& world): item_(std::move(item)), Entity(world)
{
    type_ = EntityType::Item;
}