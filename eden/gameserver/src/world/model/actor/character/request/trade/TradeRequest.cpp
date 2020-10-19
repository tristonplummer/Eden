#include <shaiya/common/net/packet/game/CharacterTradeConfirm.hpp>
#include <shaiya/common/net/packet/game/CharacterTradeGold.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/world/model/actor/character/Character.hpp>
#include <shaiya/game/world/model/actor/character/request/trade/TradeRequest.hpp>

#include <utility>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * The maximum item capacity of the trade window.
 */
constexpr auto TradeWindowCapacity = 8;

/**
 * A helper function to get the trade request instance for a character.
 * @param character The character to get the trade request for.
 * @return          The trade request.
 */
auto tradeRequest = [](const std::shared_ptr<Character>& character) -> std::shared_ptr<TradeRequest> {
    auto request = character->getAttribute<std::shared_ptr<Request>>(Attribute::Request, nullptr);
    return std::dynamic_pointer_cast<TradeRequest>(request);
};

/**
 * Initialises this request.
 * @param player    The player who made the request.
 * @param partner   The request partner.
 */
TradeRequest::TradeRequest(std::shared_ptr<Character> player, std::shared_ptr<Character> partner)
    : Request(std::move(player), std::move(partner))
{
    type_ = RequestType::Trade;

    // Initialise the trade container
    container_ = std::make_unique<ItemContainer>(TradeWindowCapacity);
}

/**
 * Opens this request.
 */
void TradeRequest::open()
{
    // Inform the player that the trade has started
    player_->session().write(TradeInitiate{ .partner = static_cast<uint32_t>(partner_->id()) });
}

/**
 * Offers an amount of gold into the trade window.
 * @param offeredGold   The requested amount of gold.
 */
void TradeRequest::offerGold(size_t offeredGold)
{
    unconfirm();  // We are modifying the current trade, so we should unconfirm for both participants.
    auto& inventory = player_->inventory();
    auto gold       = std::min(inventory.gold(), offeredGold);

    gold_ = gold;

    CharacterTradeGoldUpdate update{ .gold = static_cast<uint32_t>(gold_) };
    player_->session().write(update);

    update.participant = Participant::Partner;
    partner_->session().write(update);
}

/**
 * Confirms the current trade window.
 */
void TradeRequest::confirm()
{
    confirmed_ = true;

    CharacterConfirmTradeUpdate update{ .confirmed = confirmed_ };
    player_->session().write(update);

    update.participant = Participant::Partner;
    partner_->session().write(update);
}

/**
 * Unconfirms the current trade window.
 */
void TradeRequest::unconfirm()
{
    auto second = tradeRequest(partner_);

    confirmed_ = false;
    if (second->confirmed_)
        second->confirmed_ = false;

    CharacterConfirmTradeUpdate playerConfirm{ .confirmed = confirmed_ };
    CharacterConfirmTradeUpdate partnerConfirm{ .confirmed = second->confirmed_ };

    player_->session().write(playerConfirm);
    partner_->session().write(partnerConfirm);

    playerConfirm.participant  = Participant::Partner;
    partnerConfirm.participant = Participant::Partner;

    player_->session().write(partnerConfirm);
    partner_->session().write(playerConfirm);
}

/**
 * Cancels the trade.
 */
void TradeRequest::cancel()
{
    partner_->session().write(CharacterTradeCancelled{});
    close();
}