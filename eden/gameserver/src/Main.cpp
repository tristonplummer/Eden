#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/net/GameTcpServer.hpp>

#include <glog/logging.h>

/**
 * The entry point for the game server.
 * @param argc  The number of arguments.
 * @param argv  The command-line argument values.
 * @return      The return code.
 */
int main(int argc, char** argv)
{
    google::InitGoogleLogging(argv[0]);

    // The service context
    shaiya::game::ServiceContext ctx;

    // Initialise the tcp server to listen on a specific port with the service context.
    shaiya::net::GameTcpServer server(30810, ctx);
    server.start();
    return 0;
}