#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/world/model/item/GroundItem.hpp>

using namespace shaiya::net;
using namespace shaiya::game;

/**
 * Handles an incoming remove item request.
 * @param session   The session instance.
 * @param request   The inbound remove item request.
 */
void handleRemoveItem(Session& session, const CharacterRemoveItemRequest& request)
{
    auto& game     = dynamic_cast<GameSession&>(session);
    auto character = game.character();

    auto& inventory = character->inventory();

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
    item = inventory.remove(page, slot, request.count);
    item->setCount(request.count);

    // Create a ground item instance at the player's position
    auto groundItem = std::make_shared<GroundItem>(std::move(item));
    groundItem->setPosition(character->position());
}

/**
 * A template specialization used for registering a remove item handler.
 */
template<>
void PacketRegistry::registerPacketHandler<CharacterRemoveItemRequestOpcode>()
{
    registerHandler<CharacterRemoveItemRequestOpcode, CharacterRemoveItemRequest>(&handleRemoveItem);
}