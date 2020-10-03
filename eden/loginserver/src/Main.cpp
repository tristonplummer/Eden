#include <shaiya/login/net/LoginTcpServer.hpp>

#include <boost/property_tree/ini_parser.hpp>

/**
 * The entry point for the login server.
 * @param argc  The number of arguments.
 * @param argv  The command-line argument values.
 * @return      The return code.
 */
int main(int argc, char** argv)
{
    // Parse the configuration file
    boost::property_tree::ptree config;
    boost::property_tree::ini_parser::read_ini("./data/config/Login.ini", config);

    // The service context
    shaiya::login::ServiceContext ctx(config);

    // Initialise the tcp server to listen on a specific port with the service context.
    auto port = config.get<uint16_t>("Network.Port");
    shaiya::net::LoginTcpServer server(port, ctx);
    server.start();
    return 0;
}