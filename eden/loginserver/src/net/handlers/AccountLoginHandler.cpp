#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/login/net/LoginSession.hpp>

using namespace shaiya::net;

/**
 * Handles an incoming login request.
 * @param session   The session instance.
 * @param request   The inbound login request.
 */
void handleLogin(Session& session, const AccountLoginRequest& request)
{
    auto& login = dynamic_cast<LoginSession&>(session);
    LOG(INFO) << "[name=" << request.username.str() << ", pass=" << request.password.str() << ", ip=" << session.remoteAddress() << "]";

    auto& world = login.context().getWorldService();
    world.sendWorldList(login);

    // Dummy login response
    LoginResponse response;
    response.status = 0;
    login.write(response);
}

/**
 * A template specialization used for registering a login request handler.
 */
template<>
void PacketRegistry::registerPacketHandler<LoginRequestOpcode>()
{
    registerHandler<LoginRequestOpcode, AccountLoginRequest>(&handleLogin, ExecutionType::Asynchronous);
}