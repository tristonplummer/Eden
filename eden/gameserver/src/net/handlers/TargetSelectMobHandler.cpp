#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/TargetSelectMob.hpp>
#include <shaiya/game/model/actor/combat/CombatBuilder.hpp>
#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/ServiceContext.hpp>

using namespace shaiya::net;
using namespace shaiya::game;

/**
 * Handles an incoming mob target request.
 * @param session   The session instance.
 * @param request   The inbound mob target request.
 */
void handleTargetMob(Session& session, const TargetSelectMob& request)
{
    auto& game  = dynamic_cast<GameSession&>(session);
    auto player = game.player();

    auto& pos = player->position();
    auto map  = player->map();
    auto mob  = map->get<Mob>(pos, request.id, EntityType::Mob);

    if (!mob)
        return;

    game.write(mob->state());
    player->combat().target(mob);
}

/**
 * A template specialization used for registering a mob target selection.
 */
template<>
void PacketRegistry::registerPacketHandler<TargetSelectMobOpcode>()
{
    registerHandler<TargetSelectMobOpcode, TargetSelectMob>(&handleTargetMob);
}