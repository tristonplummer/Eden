#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/CharacterMoveItem.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/world/model/actor/player/Player.hpp>
#include <shaiya/game/world/model/item/Item.hpp>

using namespace shaiya::net;
using namespace shaiya::game;

/**
 * Equipment is always page 0.
 */
constexpr auto EquipmentPage = 0;

/**
 * Handles an incoming move item request.
 * @param session   The session instance.
 * @param request   The inbound move item request.
 */
void handleItemMove(Session& session, const CharacterMoveItemRequest& request)
{
    auto& game  = dynamic_cast<GameSession&>(session);
    auto player = game.player();

    // Get the character's containers.
    auto& inventory       = static_cast<ItemContainer&>(player->inventory());
    auto& equipment       = static_cast<ItemContainer&>(player->equipment());
    ItemContainer& source = request.sourcePage == EquipmentPage ? equipment : inventory;
    ItemContainer& dest   = request.destPage == EquipmentPage ? equipment : inventory;

    // Validate the slot and page
    if (request.sourcePage > source.pageCount())
        return;
    if (request.sourceSlot >= source.pageSize())
        return;
    if (request.destPage > dest.pageCount())
        return;
    if (request.destSlot >= dest.pageSize())
        return;

    // The source and destination pages
    auto sourcePage = request.sourcePage;
    auto sourceSlot = request.sourceSlot;
    auto destPage   = request.destPage;
    auto destSlot   = request.destSlot;

    // Decrement the pages if we're using the inventory
    if (request.sourcePage != EquipmentPage)
        sourcePage--;
    if (request.destPage != EquipmentPage)
        destPage--;

    // Transfer the items
    bool success                = false;
    auto [destItem, sourceItem] = source.transferTo(dest, source.pagePositionToIndex(sourcePage, sourceSlot),
                                                    dest.pagePositionToIndex(destPage, destSlot), success);

    // If the items weren't moved, do nothing
    if (!success)
        return;

    // Write the source item
    CharacterMoveItemUpdate update;
    auto& srcUpdate = update.source;
    srcUpdate.bag   = request.sourcePage;
    srcUpdate.slot  = request.sourceSlot;
    if (sourceItem)
    {
        srcUpdate.type       = sourceItem->type();
        srcUpdate.typeId     = sourceItem->typeId();
        srcUpdate.quantity   = sourceItem->quantity();
        srcUpdate.durability = sourceItem->durability();
    }

    // Write the destination item
    auto& destUpdate = update.destination;
    destUpdate.bag   = request.destPage;
    destUpdate.slot  = request.destSlot;
    if (destItem)
    {
        destUpdate.type       = destItem->type();
        destUpdate.typeId     = destItem->typeId();
        destUpdate.quantity   = destItem->quantity();
        destUpdate.durability = destItem->durability();
    }
    game.write(update);
}

/**
 * A template specialization used for registering a move item handler.
 */
template<>
void PacketRegistry::registerPacketHandler<CharacterMoveItemOpcode>()
{
    registerHandler<CharacterMoveItemOpcode, CharacterMoveItemRequest>(&handleItemMove);
}