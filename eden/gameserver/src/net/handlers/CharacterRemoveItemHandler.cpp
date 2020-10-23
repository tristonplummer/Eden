#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/CharacterRemoveItem.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/service/ServiceContext.hpp>
#include <shaiya/game/world/model/actor/player/Player.hpp>
#include <shaiya/game/world/model/item/GroundItem.hpp>
#include <shaiya/game/world/model/item/Item.hpp>

using namespace shaiya::net;
using namespace shaiya::game;

/**
 * Handles an incoming remove item request.
 * @param session   The session instance.
 * @param request   The inbound remove item request.
 */
void handleRemoveItem(Session& session, const CharacterRemoveItemRequest& request)
{
    auto& game  = dynamic_cast<GameSession&>(session);
    auto& world = game.context().getGameWorld();
    auto player = game.player();

    auto& inventory = player->inventory();

    auto page = request.page;
    auto slot = request.slot;

    if (page == 0 || page > inventory.pageCount())
        return;
    if (slot >= inventory.pageSize())
        return;
    page--;

    auto item = inventory.at(page, slot);
    if (!item)
        return;

    // The item that was removed
    item = inventory.remove(page, slot, request.quantity);
    item = std::make_shared<Item>(item->definition());
    item->setQuantity(request.quantity);

    // Create a ground item instance and register it to the game world
    auto groundItem = std::make_shared<GroundItem>(std::move(item), world);
    world.registerItem(groundItem);

    // Place the item at the character's feet.
    groundItem->setPosition(player->position());
}

/**
 * A template specialization used for registering a remove item handler.
 */
template<>
void PacketRegistry::registerPacketHandler<CharacterRemoveItemRequestOpcode>()
{
    registerHandler<CharacterRemoveItemRequestOpcode, CharacterRemoveItemRequest>(&handleRemoveItem);
}