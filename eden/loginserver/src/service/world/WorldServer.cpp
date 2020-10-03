#include <shaiya/login/net/LoginSession.hpp>

#include <boost/algorithm/string.hpp>

#include <sstream>
#include <vector>

using namespace shaiya::login;

/**
 * The maximum reconnect backoff, in milliseconds.
 */
constexpr auto MaxReconnectBackoff = 5000;

/**
 * Initialises a representation of a remote world server.
 * @param id                The id of the server.
 * @param name              The name of the server.
 * @param ipAddress         The ip address of the server.
 * @param apiPort           The port that the world server's api is listening on.
 * @param revision          The client revision to accept.
 * @param playerCapacity    The maximum capacity of online players.
 */
WorldServer::WorldServer(uint8_t id, std::string name, std::string ipAddress, uint16_t apiPort, uint32_t revision,
                         uint16_t playerCapacity)
    : id_(id), name_(std::move(name)), ipAddress_(std::move(ipAddress)), revision_(revision), playerCapacity_(playerCapacity)
{
    // The individual bytes of the ip address string
    std::vector<std::string> bytes;

    // Split the ip address string by it's periods
    boost::split(bytes, ipAddress_, [](char c) { return c == '.'; });

    // Store the ip address as an array of bytes
    for (auto i = 0; i < bytes.size(); i++)
        ipAddressBytes_.at(i) = std::stoi(bytes.at(i));

    // Configure the arguments, as we want a relatively low backoff
    grpc::ChannelArguments args;
    args.SetInt(GRPC_ARG_MAX_RECONNECT_BACKOFF_MS, MaxReconnectBackoff);

    // Initialise the channel
    auto endpoint = boost::format("%1%:%2%") % ipAddress_ % apiPort;
    channel_      = grpc::CreateCustomChannel(endpoint.str(), grpc::InsecureChannelCredentials(), args);
    channel_->GetState(true);

    // Initialise the client
    client_ = gameapi::GameService::NewStub(channel_);
}

/**
 * Submits a transfer request for a given session. This is sent just before the session disconnects from
 * this login server, and should connect to this world server.
 * @param session   The session that is being transferred
 * @return          If the transfer request was accepted
 */
bool WorldServer::submitTransferRequest(shaiya::net::LoginSession& session)
{
    using namespace grpc;
    ClientContext context;
    gameapi::SessionTransferRequest request;
    gameapi::SessionTransferResponse response;

    // The session's details
    auto identity  = session.identity();
    auto aesKey    = session.aesKey();
    auto aesIv     = session.aesIv();
    auto ipAddress = session.remoteAddress();

    // Build the request
    request.set_userid(session.userId());
    request.set_identity(identity.data(), identity.size());
    request.set_ipaddress(ipAddress);
    request.set_key(aesKey.data(), aesKey.size());
    request.set_iv(aesIv.data(), aesIv.size());

    // Send the request and return the response
    auto status = client_->SubmitSessionTransfer(&context, request, &response);
    return status.ok() && response.status() == gameapi::SessionTransferStatus::Success;
}

/**
 * Queries the remote world server, and updates our internal data.
 */
void WorldServer::update()
{
    using namespace grpc;
    ClientContext context;
    gameapi::Void empty;
    gameapi::WorldStatus worldStatus;

    auto status = client_->GetWorldStatus(&context, empty, &worldStatus);
    if (!status.ok())
    {
        online_      = false;
        playerCount_ = 0;
        return;
    }

    online_      = true;
    playerCount_ = worldStatus.players();
}