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
    auto& auth  = login.context().getAuthService();

    // Process the login request
    auth.login(login, request.username.str(), request.password.str());
}

/**
 * A template specialization used for registering a login request handler.
 */
template<>
void PacketRegistry::registerPacketHandler<LoginRequestOpcode>()
{
    registerHandler<LoginRequestOpcode, AccountLoginRequest>(&handleLogin, ExecutionType::Asynchronous);
}