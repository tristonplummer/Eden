#include <shaiya/login/net/LoginTcpServer.hpp>

#include <memory>

using namespace shaiya::net;

/**
 * Creates a login session from an io context.
 * @param ioContext The io context.
 * @return          The session instance.
 */
std::shared_ptr<LoginSession> LoginTcpServer::createSession(boost::asio::io_context& ioContext)
{
    return std::make_shared<LoginSession>(ioContext, ctx_);
}