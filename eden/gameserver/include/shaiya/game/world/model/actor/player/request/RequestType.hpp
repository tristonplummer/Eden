#pragma once

namespace shaiya::game
{
    /**
     * Represents the various types of requests that can be made by player characters.
     */
    enum class RequestType
    {
        /**
         * If no request is present.
         */
        None,

        /**
         * A request for opening a trade window between two players.
         */
        Trade
    };
}