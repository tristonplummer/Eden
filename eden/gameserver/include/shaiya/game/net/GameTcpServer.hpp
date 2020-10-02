#pragma once
#include <shaiya/common/net/TcpServer.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/ServiceContext.hpp>

namespace shaiya::net
{
    /**
     * A TcpServer instance that accepts incoming game sessions.
     */
    class GameTcpServer: public TcpServer<GameSession>
    {
    public:
        /**
         * Initialises this game server to listen on a specific port.
         * @param port  The port for the game server to listen on.
         * @param ctx   The service context to provide to sessions.
         */
        explicit GameTcpServer(uint16_t port, shaiya::game::ServiceContext& ctx): TcpServer(port), ctx_(ctx)
        {
        }

    private:
        /**
         * Creates a game session from an io context.
         * @param ioContext The io context.
         * @return          The session instance.
         */
        std::shared_ptr<GameSession> createSession(boost::asio::io_context& ioContext) override;

        /**
         * The game service context.
         */
        shaiya::game::ServiceContext& ctx_;
    };
}