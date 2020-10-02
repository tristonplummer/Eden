#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/login/net/LoginSession.hpp>

using namespace shaiya::net;

/**
 * Handles an incoming world selection request.
 * @param session   The session instance.
 * @param request   The inbound world select request.
 */
void handleWorldSelect(Session& session, const WorldSelectRequest& request)
{
    auto& login        = dynamic_cast<LoginSession&>(session);
    auto& worldService = login.context().getWorldService();

    /**
     * Sends an error response to the session.
     */
    auto sendError = [&](WorldSelectStatus status) {
        WorldSelectResponse response;
        response.status = status;
        login.write(response, 3);
    };

    auto* world = worldService.getWorld(request.id);  // Get the world with the specified id.
    if (!world)
        return sendError(WorldSelectStatus::CannotConnect);
    if (!world->isOnline())
        return sendError(WorldSelectStatus::CannotConnect);
    if (world->isFull())
        return sendError(WorldSelectStatus::ServerSaturated);
    if (request.version != world->revision())
        return sendError(WorldSelectStatus::VersionDoesntMatch);

    // Request that the world accepts this session as a transfer
    if (!world->submitTransferRequest(login))
        return sendError(WorldSelectStatus::TryAgainLater);

    // Send the successful connection response
    WorldSelectResponse response;
    response.status    = WorldSelectStatus::Success;
    response.ipAddress = world->ipAddress();
    login.write(response);
}

/**
 * A template specialization used for registering a world select request handler.
 */
template<>
void PacketRegistry::registerPacketHandler<WorldSelectOpcode>()
{
    registerHandler<WorldSelectOpcode, WorldSelectRequest>(&handleWorldSelect, ExecutionType::Asynchronous);
}