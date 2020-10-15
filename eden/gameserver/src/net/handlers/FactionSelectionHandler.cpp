#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/AccountFaction.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/CharacterScreenService.hpp>
#include <shaiya/game/service/ServiceContext.hpp>

using namespace shaiya::net;

/**
 * Handles an incoming faction selection request.
 * @param session   The session instance.
 * @param request   The inbound faction selection request.
 */
void handleFactionSelect(Session& session, const AccountFactionSelectRequest& request)
{
    auto& game       = dynamic_cast<GameSession&>(session);
    auto& charScreen = game.context().getCharScreen();

    if (!charScreen.setFaction(game, request.faction))
        return game.close();
    charScreen.display(game);
}

/**
 * A template specialization used for registering a faction selection request handler.
 */
template<>
void PacketRegistry::registerPacketHandler<AccountFactionOpcode>()
{
    registerHandler<AccountFactionOpcode, AccountFactionSelectRequest>(&handleFactionSelect, ExecutionType::Asynchronous);
}