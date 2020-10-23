#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/world/model/actor/character/request/RequestType.hpp>

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

        /**
         * Gets the partner of this request.
         * @return  The partner.
         */
        [[nodiscard]] const std::shared_ptr<Character>& partner() const
        {
            return partner_;
        }

        /**
         * Gets the type of this request.
         * @return  The type.
         */
        [[nodiscard]] RequestType type() const
        {
            return type_;
        }

    protected:
        /**
         * The player who made the request.
         */
        std::shared_ptr<Character> player_;

        /**
         * The request partner.
         */
        std::shared_ptr<Character> partner_;

        /**
         * The type of this request.
         */
        RequestType type_{ RequestType::None };
    };
}