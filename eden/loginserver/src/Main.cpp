#include <shaiya/login/net/LoginTcpServer.hpp>

int main(int argc, char** argv)
{
    // The service context
    shaiya::login::ServiceContext ctx;

    // Initialise the tcp server to listen on a specific port with the service context.
    shaiya::net::LoginTcpServer server(30800, ctx);
    server.start();
    return 0;
}