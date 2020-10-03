#include <shaiya/game/service/WorldApiService.hpp>

#include <boost/format.hpp>

using namespace shaiya::game;
using namespace gameapi;
using namespace grpc;

/**
 * The local ip address to listen on.
 */
constexpr auto IpAddress = "0.0.0.0";

/**
 * Starts this api service and listens for connections on a specified port.
 * @param port  The port to listen on.
 */
void WorldApiService::start(uint16_t port)
{
    // The local endpoint to listen on
    auto endpoint = boost::format("%1%:%2%") % IpAddress % port;

    // Build this service
    ServerBuilder bldr;
    bldr.AddListeningPort(endpoint.str(), grpc::InsecureServerCredentials());
    bldr.RegisterService(this);

    // Listen for incoming connections
    auto server = bldr.BuildAndStart();
    server->Wait();
}

/**
 * Gets the status of this game world.
 * @param context   The context of this server.
 * @param request   The request, which for this case is just an empty message.
 * @param response  The status of the game world.
 * @return          The status of the request.
 */
Status WorldApiService::GetWorldStatus(ServerContext* context, const Void* request, WorldStatus* response)
{
    response->set_players(5);
    return Status::OK;
}

/**
 * Handles an incoming session transfer request.
 * @param context   The context of this server.
 * @param request   The transfer request.
 * @param response  The transfer response, which is used to tell the login server if we accepted it or not.
 * @return          The status of the request.
 */
Status WorldApiService::SubmitSessionTransfer(ServerContext* context, const SessionTransferRequest* request,
                                              SessionTransferResponse* response)
{
    response->set_status(SessionTransferStatus::Success);
    return Status::OK;
}
