#include <shaiya/login/service/world/WorldServer.hpp>

#include <boost/algorithm/string.hpp>

#include <sstream>
#include <vector>

using namespace shaiya::login;

/**
 * The port that the world server api is listening on.
 */
constexpr auto ApiRequestPort = 8080;

/**
 * The maximum reconnect backoff, in milliseconds.
 */
constexpr auto MaxReconnectBackoff = 5000;

/**
 * Initialises a representation of a remote world server.
 * @param id                The id of the server.
 * @param name              The name of the server.
 * @param ipAddress         The ip address of the server.
 * @param revision          The client revision to accept.
 * @param playerCapacity    The maximum capacity of online players.
 */
WorldServer::WorldServer(uint8_t id, std::string name, std::string ipAddress, uint32_t revision, uint16_t playerCapacity)
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
    auto endpoint = boost::format("%1%:%2%") % ipAddress_ % ApiRequestPort;
    channel_      = grpc::CreateCustomChannel(endpoint.str(), grpc::InsecureChannelCredentials(), args);

    // Initialise the client
    client_ = gameapi::GameService::NewStub(channel_);
}

/**
 * Queries the remote world server, and updates our internal data.
 */
void WorldServer::update()
{
    using namespace grpc;
    ClientContext context;
    gameapi::Void empty;
    gameapi::PlayerCount players;

    auto status = client_->GetPlayerCount(&context, empty, &players);
    if (!status.ok())
    {
        online_      = false;
        playerCount_ = 0;
        return;
    }

    online_      = true;
    playerCount_ = players.players();
}