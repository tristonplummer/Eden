#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/MobState.hpp>
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
void handleMobState(Session& session, const RequestMobState& request)
{
    auto& game  = dynamic_cast<GameSession&>(session);
    auto player = game.player();

    auto& pos = player->position();
    auto map  = player->map();
    auto mob  = map->get<Actor>(pos, request.id, EntityType::Mob);

    if (!mob)
        return;

    // The state of the mob
    MobState state;
    state.id        = mob->id();
    state.hitpoints = mob->stats().currentHitpoints();
    game.write(state);
}

/**
 * A template specialization used for registering a mob target selection.
 */
template<>
void PacketRegistry::registerPacketHandler<MobStateOpcode>()
{
    registerHandler<MobStateOpcode, RequestMobState>(&handleMobState);
}