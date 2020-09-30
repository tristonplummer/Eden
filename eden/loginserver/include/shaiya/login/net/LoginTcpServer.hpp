#pragma once
#include <shaiya/common/net/TcpServer.hpp>
#include <shaiya/login/net/LoginSession.hpp>
#include <shaiya/login/service/ServiceContext.hpp>

namespace shaiya::net
{
    /**
     * A TcpServer instance that accepts incoming login sessions.
     */
    class LoginTcpServer: public TcpServer<LoginSession>
    {
    public:
        /**
         * Initialises this login server to listen on a specific port.
         * @param port  The port for the login server to listen on.
         * @param ctx   The service context to provide to sessions.
         */
        explicit LoginTcpServer(uint16_t port, shaiya::login::ServiceContext& ctx): TcpServer(port), ctx_(ctx)
        {
        }

    private:
        /**
         * Creates a login session from an io context.
         * @param ioContext The io context.
         * @return          The session instance.
         */
        std::shared_ptr<LoginSession> createSession(boost::asio::io_context& ioContext) override;

        /**
         * The login service context.
         */
        shaiya::login::ServiceContext& ctx_;
    };
}