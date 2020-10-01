#include <cpprest/http_client.h>
#include <shaiya/login/service/world/WorldServer.hpp>

#include <boost/algorithm/string.hpp>

#include <sstream>
#include <vector>

using namespace shaiya::login;

/**
 * The port that the world server api is listening on.
 */
constexpr auto HttpRequestPort = 8080;

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
}

/**
 * Queries the remote world server, and updates our internal data.
 */
void WorldServer::update()
{
    using namespace web::http;
    using namespace web::http::client;

    // The endpoint to query for this world server.
    std::stringstream ss;
    ss << "http://" << ipAddress_ << ":" << HttpRequestPort << "/status/";

    try
    {
        auto request = http_client(ss.str())
                           .request(methods::GET)
                           .then([&](const http_response& response) {
                               online_ = response.status_code() == status_codes::OK;
                               return response.extract_json();
                           })
                           .then([&](const web::json::value& value) { playerCount_ = value.at("players").as_integer(); });
        request.wait();
    }
    catch (const std::exception& e)  // If an exception occurs, assume the server is offline
    {
        playerCount_ = 0;
        online_      = false;
    }
}