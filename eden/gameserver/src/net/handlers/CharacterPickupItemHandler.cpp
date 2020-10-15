#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/CharacterPickupItem.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/service/ServiceContext.hpp>
#include <shaiya/game/world/model/actor/character/Character.hpp>
#include <shaiya/game/world/model/item/GroundItem.hpp>
#include <shaiya/game/world/model/map/Map.hpp>

using namespace shaiya::net;
using namespace shaiya::game;

/**
 * Handles an incoming item pickup request.
 * @param session   The session instance.
 * @param request   The inbound pickup item request.
 */
void handlePickupItem(Session& session, const CharacterPickupItemRequest& request)
{
    auto& game     = dynamic_cast<GameSession&>(session);
    auto& world    = game.context().getGameWorld();
    auto character = game.character();
    auto& pos      = character->position();

    auto map    = character->map();
    auto entity = map->get(pos, request.id, EntityType::Item);
    if (!entity)
        return;

    auto groundItem = std::dynamic_pointer_cast<GroundItem>(entity);
    auto item       = groundItem->item();
    character->inventory().add(std::move(item));

    world.unregisterItem(groundItem);
}

/**
 * A template specialization used for registering a pickup item handler.
 */
template<>
void PacketRegistry::registerPacketHandler<CharacterPickupItemRequestOpcode>()
{
    registerHandler<CharacterPickupItemRequestOpcode, CharacterPickupItemRequest>(&handlePickupItem);
}