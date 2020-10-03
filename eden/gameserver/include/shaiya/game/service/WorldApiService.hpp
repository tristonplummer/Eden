#pragma once
#include <proto/GameApi.grpc.pb.h>

#include <grpc++/grpc++.h>

namespace shaiya::game
{
    /**
     * The local world api service. This is primarily used by the login server for checking the world server's status,
     * and the number of players that are online, as well as reasoning about session transfers that we should expect to
     * receive.
     */
    class WorldApiService: public gameapi::GameService::Service
    {
    public:
        /**
         * Starts this api service and listens for connections on a specified port.
         * @param port  The port to listen on.
         */
        void start(uint16_t port);

        /**
         * Gets the status of this game world.
         * @param context   The context of this server.
         * @param request   The request, which for this case is just an empty message.
         * @param response  The status of the game world.
         * @return          The status of the request.
         */
        grpc::Status GetWorldStatus(grpc::ServerContext* context, const gameapi::Void* request,
                                    gameapi::WorldStatus* response) override;

        /**
         * Handles an incoming session transfer request.
         * @param context   The context of this server.
         * @param request   The transfer request.
         * @param response  The transfer response, which is used to tell the login server if we accepted it or not.
         * @return          The status of the request.
         */
        grpc::Status SubmitSessionTransfer(grpc::ServerContext* context, const gameapi::SessionTransferRequest* request,
                                           gameapi::SessionTransferResponse* response) override;
    };
}