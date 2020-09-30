#pragma once
#include <shaiya/common/net/Session.hpp>

#include <boost/asio.hpp>
#include <glog/logging.h>

#include <type_traits>

namespace shaiya::net
{
    /**
     * A simple TCP server that handles the processing of inbound network events.
     * @tparam T    The session type.
     */
    template<typename T>
    class TcpServer
    {
        static_assert(std::is_base_of_v<Session, T>, "Type T must inherit from shaiya::net::Session.");

    public:
        /**
         * Initialises this TcpServer to handle incoming network events on a specific
         * local port.
         * @param port  The port to operate on.
         */
        explicit TcpServer(uint16_t port): acceptor_(ctx_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
        {
        }

        /**
         * Handles the destruction of this server.
         */
        ~TcpServer()
        {
            acceptor_.close();
        }

        /**
         * Starts this server, and begins accepting connections.
         */
        void start()
        {
            auto endpoint = acceptor_.local_endpoint();
            LOG(INFO) << "NioServer listening on " << endpoint.address().to_string() << ":" << endpoint.port();

            acceptConnection();
            ctx_.run();
        }

    private:
        /**
         * Creates a session for the specified type
         * @param ioContext The worker context
         * @return          The session instance
         */
        virtual std::shared_ptr<T> createSession(boost::asio::io_context& ioContext) = 0;

        /**
         * Begins accepting a new incoming connection.
         */
        void acceptConnection()
        {
            auto session = createSession(ctx_);
            acceptor_.async_accept(session->socket(), [this, session](const boost::system::error_code& error) {
                if (error)
                {
                    session->close();
                }
                else
                {
                    session->onAccept();
                    session->read();
                }

                acceptConnection();
            });
        }

        /**
         * The worker context to use for this server.
         */
        boost::asio::io_context ctx_;

        /**
         * The connection acceptor for this server.
         */
        boost::asio::ip::tcp::acceptor acceptor_;
    };
}