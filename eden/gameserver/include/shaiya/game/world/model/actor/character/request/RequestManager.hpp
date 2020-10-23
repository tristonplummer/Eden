#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/world/model/actor/character/request/Request.hpp>
#include <shaiya/game/world/model/actor/character/request/RequestType.hpp>

namespace shaiya::game
{
    /**
     * A utility class that handles the sending and accepting of player-made requests.
     */
    class RequestManager
    {
    public:
        /**
         * Initialises the request manager for a character.
         * @param character The character to make requests for.
         */
        explicit RequestManager(Character& character);

        /**
         * Attempts to send a request to a partner.
         * @param partner   The partner player.
         * @param type      The request type.
         * @return          If the request was made.
         */
        bool request(std::shared_ptr<Character> partner, RequestType type);

    private:
        /**
         * Checks if we can send a request to a partner.
         * @param partner   The partner.
         * @param type      The request type.
         * @return          If the request can be made.
         */
        bool canRequest(const std::shared_ptr<Character>& partner, RequestType type);

        /**
         * Attempts to accept an existing request from a partner.
         * @param partner   The request partner.
         * @param type      The request type.
         * @return          If an existing request was accepted.
         */
        bool acceptExisting(const std::shared_ptr<Character>& partner, RequestType type);

        /**
         * Gets the request for a specific type.
         * @param type      The request type.
         * @param player    The player making the request.
         * @param partner   The partner in the request.
         * @return          The request.
         */
        static std::shared_ptr<Request> forType(RequestType type, std::shared_ptr<Character> player,
                                                std::shared_ptr<Character> partner);

        /**
         * The character who we are managing requests for.
         */
        Character& character_;

        /**
         * The current request partner.
         */
        std::shared_ptr<Character> partner_;
    };
}