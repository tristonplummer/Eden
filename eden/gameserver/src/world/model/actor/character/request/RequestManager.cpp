#include <shaiya/game/world/model/actor/character/Character.hpp>
#include <shaiya/game/world/model/actor/character/request/trade/TradeRequest.hpp>

using namespace shaiya::game;

/**
 * Initialises the request manager for a character.
 * @param character The character to make requests for.
 */
RequestManager::RequestManager(Character& character): character_(character)
{
}

/**
 * Attempts to send a request to a partner.
 * @param partner   The partner player.
 * @param type      The request type.
 * @return          If the request was made.
 */
bool RequestManager::request(std::shared_ptr<Character> partner, RequestType type)
{
    if (!canRequest(partner, type))
        return false;
    if (acceptExisting(partner, type))
        return false;

    character_.setAttribute(Attribute::LastRequest, type);
    partner_ = std::move(partner);
    partner_->setAttribute(Attribute::LastRequestingCharacter, character_.id());
    return true;
}

/**
 * Checks if we can send a request to a partner.
 * @param partner   The partner.
 * @param type      The request type.
 * @return          If the request can be made.
 */
bool RequestManager::canRequest(const std::shared_ptr<Character>& partner, RequestType type)
{
    if (character_.id() == partner->id())  // We don't want players to be able to send requests to themselves.
        return false;
    if (!partner->active())  // We don't want to send requests to a player that hasn't loaded in yet.
        return false;
    return !partner->hasAttribute(Attribute::Request);
}

/**
 * Attempts to accept an existing request from a partner.
 * @param partner   The partner.
 * @param type      The request type.
 * @return          If an existing request was accepted.
 */
bool RequestManager::acceptExisting(const std::shared_ptr<Character>& partner, RequestType type)
{
    auto lastType = partner->getAttribute(Attribute::LastRequest, RequestType::None);
    if (lastType == type)
    {
        auto player = partner->requests().partner_;     // The target of our target.
        if (player && player->id() == character_.id())  // If the target of our target, is us.
        {
            auto first  = forType(type, player, partner);
            auto second = forType(type, partner, player);

            player->setAttribute(Attribute::Request, first);
            partner->setAttribute(Attribute::Request, second);

            first->open();
            second->open();
            return true;
        }
    }
    return false;
}

/**
 * Gets the request for a specific type.
 * @param type      The request type.
 * @param player    The player making the request.
 * @param partner   The partner in the request.
 * @return          The request.
 */
std::shared_ptr<Request> RequestManager::forType(RequestType type, std::shared_ptr<Character> player,
                                                 std::shared_ptr<Character> partner)
{
    switch (type)
    {
        case RequestType::None: return nullptr;
        case RequestType::Trade: return std::make_shared<TradeRequest>(player, partner);
    }
    return nullptr;
}