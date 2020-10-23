#include <shaiya/game/model/actor/player/request/trade/TradeRequest.hpp>
#include <shaiya/game/model/item/container/ItemContainer.hpp>
#include <shaiya/game/model/item/container/event/TradeEventListener.hpp>

using namespace shaiya::game;

/**
 * Initialises the trade event listener.
 * @param player The character to listen for.
 * @param trade     The trade session.
 */
TradeEventListener::TradeEventListener(std::shared_ptr<Player> player, TradeRequest& trade)
    : player_(std::move(player)), trade_(trade)
{
}

/**
 * Gets executed when an item is moved from the inventory.
 * @param source        The source container.
 * @param dest          The destination container.
 * @param sourceSlot    The source slot.
 * @param destSlot      The destination slot.
 */
void TradeEventListener::itemTransferred(const ItemContainer& source, const ItemContainer& dest, size_t sourceSlot,
                                         size_t destSlot)
{
    trade_.itemMoved(sourceSlot);
}