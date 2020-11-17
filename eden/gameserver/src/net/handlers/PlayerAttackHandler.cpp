#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/PlayerAttackStart.hpp>
#include <shaiya/common/net/packet/game/PlayerAutoAttack.hpp>
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
void handleAutoAttack(Session& session, const PlayerAutoAttackRequest& request)
{
    auto& game  = dynamic_cast<GameSession&>(session);
    auto player = game.player();
    auto map    = player->map();
    auto& pos   = player->position();

    if (!map)
        return;

    auto fail = [&](AttackStatus status, const std::shared_ptr<Actor>& target = nullptr) {
        PlayerAutoAttack attack;
        attack.opcode = request.opcode;
        attack.id     = player->id();
        attack.target = target == nullptr ? 0 : target->id();
        attack.status = status;
        game.write(attack);
    };

    auto type   = request.opcode == PlayerAutoAttackPlayerOpcode ? EntityType::Player : EntityType::Mob;
    auto target = map->get<Actor>(pos, request.target, type);

    if (!target)
        return fail(AttackStatus::WrongTarget);

    auto wep = player->equipment().at(EquipmentSlot::Weapon);
    if (wep == nullptr)
        return fail(AttackStatus::CannotAttack, target);

    PlayerAttackStart start;
    game.write(start);

    player->combat().attack(target);
}

/**
 * A template specialization used for registering a player auto attack request.
 */
template<>
void PacketRegistry::registerPacketHandler<PlayerAutoAttackPlayerOpcode>()
{
    registerHandler<PlayerAutoAttackPlayerOpcode, PlayerAutoAttackRequest>(&handleAutoAttack);
}

/**
 * A template specialization used for registering a player auto attack request.
 */
template<>
void PacketRegistry::registerPacketHandler<PlayerAutoAttackMobOpcode>()
{
    registerHandler<PlayerAutoAttackMobOpcode, PlayerAutoAttackRequest>(&handleAutoAttack);
}