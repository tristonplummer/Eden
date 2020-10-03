#include <shaiya/game/service/WorldApiService.hpp>

#include <boost/format.hpp>
#include <glog/logging.h>

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
    // Make a copy of the request and store it in our pending transfers
    auto transfer = new SessionTransferRequest(*request);
    pendingTransfers_.push_back(transfer);

    // Inform the login server that we accepted this transfer request.
    response->set_status(SessionTransferStatus::Success);
    return Status::OK;
}

/**
 * Gets a transfer request for a given identity
 * @param identity  The identity
 * @return          The transfer request.
 */
std::unique_ptr<gameapi::SessionTransferRequest> WorldApiService::getTransferForIdentity(std::array<char, 16> identity)
{
    std::unique_ptr<gameapi::SessionTransferRequest> request = nullptr;
    for (auto&& transfer: pendingTransfers_)
    {
        // Get the transfer identity
        auto transferIdentity = transfer->identity();
        if (std::memcmp(identity.data(), transferIdentity.data(), identity.size()) == 0)
        {
            request.reset(transfer);
            break;
        }
    }

    // If a request was found, remove it from the pending vector
    if (request)
    {
        auto pos = std::find(pendingTransfers_.begin(), pendingTransfers_.end(), request.get());
        if (pos != pendingTransfers_.end())
        {
            pendingTransfers_.erase(pos);
        }
    }
    return request;
}