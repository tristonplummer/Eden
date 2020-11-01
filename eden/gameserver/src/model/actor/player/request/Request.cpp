#include <shaiya/game/model/actor/player/Player.hpp>

using namespace shaiya::game;

/**
 * Initialises this request.
 * @param player    The player who made the request.
 * @param partner   The request partner.
 */
Request::Request(std::shared_ptr<Player> player, std::shared_ptr<Player> partner)
    : player_(std::move(player)), partner_(std::move(partner))
{
}

/**
 * Closes this request.
 */
void Request::close()
{
    player_->clearAttribute(Attribute::Request);

    auto partnerRequest = partner_->getAttribute<std::shared_ptr<Request>>(Attribute::Request, nullptr);
    if (partnerRequest)
    {
        partnerRequest->close();
    }
}