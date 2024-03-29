#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/CharacterTradeConfirm.hpp>
#include <shaiya/common/net/packet/game/CharacterTradeGold.hpp>
#include <shaiya/common/net/packet/game/CharacterTradeItem.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/actor/player/request/trade/TradeRequest.hpp>
#include <shaiya/game/model/map/Map.hpp>

using namespace shaiya::net;
using namespace shaiya::game;

/**
 * Handles an incoming trade request.
 * @param session   The session instance.
 * @param request   The inbound trade request.
 */
void handleTradeRequest(Session& session, const CharacterTradeRequest& request)
{
    auto& game  = dynamic_cast<GameSession&>(session);
    auto player = game.player();

    auto& pos   = player->position();
    auto map    = player->map();
    auto target = map->get<Player>(pos, request.target, EntityType::Player);

    if (!target)
        return;

    // If we can send a request to the target
    auto& requests = player->requests();
    if (!requests.request(target, RequestType::Trade))
    {
        return;
    }

    CharacterTradeRequest outgoingRequest;
    outgoingRequest.target = player->id();
    target->session().write(outgoingRequest);
}

/**
 * Handles an incoming trade response.
 * @param session   The session instance.
 * @param request   The inbound trade response.
 */
void handleTradeResponse(Session& session, const CharacterTradeResponse& response)
{
    auto& game     = dynamic_cast<GameSession&>(session);
    auto character = game.player();

    auto id        = character->getAttribute<size_t>(Attribute::LastRequestingCharacter, 0);
    auto& pos      = character->position();
    auto map       = character->map();
    auto& requests = character->requests();
    auto target    = map->get<Player>(pos, id, EntityType::Player);

    if (!target)
        return;

    if (!response.accepted)
    {
        return;
    }

    requests.request(target, RequestType::Trade);
}

/**
 * Handles an incoming trade finalisation request.
 * @param session   The session instance.
 * @param request   The inbound trade finalisation request.
 */
void handleTradeFinalise(Session& session, const CharacterTradeFinaliseRequest& req)
{
    auto& game     = dynamic_cast<GameSession&>(session);
    auto character = game.player();

    auto request = character->getAttribute<std::shared_ptr<Request>>(Attribute::Request, nullptr);
    if (!request || request->type() != RequestType::Trade)
        return;

    auto trade = std::dynamic_pointer_cast<TradeRequest>(request);

    if (req.type == TradeFinaliseType::Accepted)
        trade->accept();
    else if (req.type == TradeFinaliseType::Cancel)
        trade->close();
}

/**
 * Handles an incoming trade confirmation request.
 * @param session   The session instance.
 * @param request   The inbound trade confirmation request.
 */
void handleTradeConfirm(Session& session, const CharacterConfirmTrade& req)
{
    auto& game     = dynamic_cast<GameSession&>(session);
    auto character = game.player();

    auto request = character->getAttribute<std::shared_ptr<Request>>(Attribute::Request, nullptr);
    if (!request || request->type() != RequestType::Trade)
        return;

    auto trade = std::dynamic_pointer_cast<TradeRequest>(request);

    if (req.confirmed)
        trade->confirm();
    else
        trade->unconfirm();
}

/**
 * Handles an incoming offer to add gold to the trade window.
 * @param session   The session instance.
 * @param request   The inbound trade offer.
 */
void handleTradeGoldOffer(Session& session, const CharacterTradeOfferGoldRequest& offer)
{
    auto& game     = dynamic_cast<GameSession&>(session);
    auto character = game.player();

    auto request = character->getAttribute<std::shared_ptr<Request>>(Attribute::Request, nullptr);
    if (!request || request->type() != RequestType::Trade)
        return;

    auto trade = std::dynamic_pointer_cast<TradeRequest>(request);
    trade->offerGold(offer.gold);
}

/**
 * Handles an incoming offer to add an item to the trade window.
 * @param session   The session instance.
 * @param request   The inbound trade offer.
 */
void handleTradeItemOffer(Session& session, const CharacterTradeOfferItemRequest& offer)
{
    auto& game     = dynamic_cast<GameSession&>(session);
    auto character = game.player();

    auto request = character->getAttribute<std::shared_ptr<Request>>(Attribute::Request, nullptr);
    if (!request || request->type() != RequestType::Trade)
        return;

    auto& inventory = character->inventory();
    auto trade      = std::dynamic_pointer_cast<TradeRequest>(request);
    auto page       = offer.page;

    if (page == 0 || page > inventory.pageCount())
        return;
    if (offer.slot >= inventory.pageSize())
        return;
    page--;

    if (trade->offerItem(inventory.pagePositionToIndex(page, offer.slot), offer.quantity, offer.destSlot))
        game.write(offer);
}

/**
 * Handles an incoming request to remove an item from the trade window.
 * @param session   The session instance.
 * @param request   The inbound removal request.
 */
void handleTradeItemRemove(Session& session, const CharacterTradeRemoveItemRequest& offer)
{
    auto& game     = dynamic_cast<GameSession&>(session);
    auto character = game.player();

    auto request = character->getAttribute<std::shared_ptr<Request>>(Attribute::Request, nullptr);
    if (!request || request->type() != RequestType::Trade)
        return;

    auto trade = std::dynamic_pointer_cast<TradeRequest>(request);
    trade->removeItem(offer.slot);

    game.write(CharacterTradeRemoveItem{ .slot = offer.slot });
}

/**
 * A template specialization used for registering a trade request handler.
 */
template<>
void PacketRegistry::registerPacketHandler<TradeRequestOpcode>()
{
    registerHandler<TradeRequestOpcode, CharacterTradeRequest>(&handleTradeRequest);
}

/**
 * A template specialization used for registering a trade response handler.
 */
template<>
void PacketRegistry::registerPacketHandler<TradeResponseOpcode>()
{
    registerHandler<TradeResponseOpcode, CharacterTradeResponse>(&handleTradeResponse);
}

/**
 * A template specialization used for registering a trade finalisation handler.
 */
template<>
void PacketRegistry::registerPacketHandler<TradeFinaliseOpcode>()
{
    registerHandler<TradeFinaliseOpcode, CharacterTradeFinaliseRequest>(&handleTradeFinalise);
}

/**
 * A template specialization used for registering a trade confirmation handler.
 */
template<>
void PacketRegistry::registerPacketHandler<TradeConfirmOpcode>()
{
    registerHandler<TradeConfirmOpcode, CharacterConfirmTrade>(&handleTradeConfirm);
}

/**
 * A template specialization used for registering a trade offer handler.
 */
template<>
void PacketRegistry::registerPacketHandler<TradeAddGoldOpcode>()
{
    registerHandler<TradeAddGoldOpcode, CharacterTradeOfferGoldRequest>(&handleTradeGoldOffer);
}

/**
 * A template specialization used for registering a trade offer handler.
 */
template<>
void PacketRegistry::registerPacketHandler<TradeOfferItemOpcode>()
{
    registerHandler<TradeOfferItemOpcode, CharacterTradeOfferItemRequest>(&handleTradeItemOffer);
}

/**
 * A template specialization used for registering a trade item removal handler.
 */
template<>
void PacketRegistry::registerPacketHandler<TradeRemoveItemOpcode>()
{
    registerHandler<TradeRemoveItemOpcode, CharacterTradeRemoveItemRequest>(&handleTradeItemRemove);
}
