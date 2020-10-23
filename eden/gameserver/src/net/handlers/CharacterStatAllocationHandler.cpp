#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/CharacterStatAllocation.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/world/model/actor/player/Player.hpp>

using namespace shaiya::net;
using namespace shaiya::game;

/**
 * Handles an incoming stat allocation request.
 * @param session   The session instance.
 * @param req       The inbound stat allocation request.
 */
void handleStatAllocation(Session& session, const CharacterStatAllocation& req)
{
    auto& game  = dynamic_cast<GameSession&>(session);
    auto player = game.player();

    // The total number of allocated stats
    auto allocated = (req.strength + req.dexterity + req.reaction + req.intelligence + req.wisdom + req.luck);
    if (allocated > player->statpoints())
        return;

    // Remove the statpoints from the character
    player->setStatpoints(player->statpoints() - allocated);

    // Add to the character's base stats
    auto& stats = player->stats();
    stats.setBase(Stat::Strength, stats.getBase(Stat::Strength) + req.strength);
    stats.setBase(Stat::Dexterity, stats.getBase(Stat::Dexterity) + req.dexterity);
    stats.setBase(Stat::Reaction, stats.getBase(Stat::Reaction) + req.reaction);
    stats.setBase(Stat::Intelligence, stats.getBase(Stat::Intelligence) + req.intelligence);
    stats.setBase(Stat::Wisdom, stats.getBase(Stat::Wisdom) + req.wisdom);
    stats.setBase(Stat::Luck, stats.getBase(Stat::Luck) + req.luck);
    stats.sync();

    // Write the request back to the character, so they know it was successful.
    game.write(req);
}

/**
 * A template specialization used for registering a stat allocation handler.
 */
template<>
void PacketRegistry::registerPacketHandler<CharacterStatAllocationOpcode>()
{
    registerHandler<CharacterStatAllocationOpcode, CharacterStatAllocation>(&handleStatAllocation);
}