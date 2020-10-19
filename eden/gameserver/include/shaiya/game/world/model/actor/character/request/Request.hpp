#pragma once
#include <shaiya/game/Forward.hpp>

namespace shaiya::game
{
    /**
     * Represents a request made from one player to another.
     */
    class Request
    {
    public:
        /**
         * Initialises this request.
         * @param player    The player who made the request.
         * @param partner   The request partner.
         */
        Request(std::shared_ptr<Character> player, std::shared_ptr<Character> partner);

        /**
         * Opens this request.
         */
        virtual void open() = 0;

        /**
         * Closes this request.
         */
        virtual void close();

    protected:
        /**
         * The player who made the request.
         */
        std::shared_ptr<Character> player_;

        /**
         * The request partner.
         */
        std::shared_ptr<Character> partner_;
    };
}