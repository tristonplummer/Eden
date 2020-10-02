#include <shaiya/game/net/GameTcpServer.hpp>

#include <memory>

using namespace shaiya::net;

/**
 * Creates a game session from an io context.
 * @param ioContext The io context.
 * @return          The session instance.
 */
std::shared_ptr<GameSession> GameTcpServer::createSession(boost::asio::io_context& ioContext)
{
    return std::make_shared<GameSession>(ioContext, ctx_);
}