#include <shaiya/common/net/packet/game/CharacterTradeItem.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/world/model/actor/character/Character.hpp>
#include <shaiya/game/world/model/item/Item.hpp>
#include <shaiya/game/world/model/item/container/event/TradeEventListener.hpp>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Initialises this event listener for a character.
 * @param player    The player who owns this container
 * @param partner   The trading partner
 */
TradeEventListener::TradeEventListener(std::shared_ptr<Character> player, std::shared_ptr<Character> partner)
    : player_(std::move(player)), partner_(std::move(partner))
{
}

/**
 * Gets executed when an item is added to a container.
 * @param container The container instance.
 * @param item      The item that was added.
 * @param slot      The slot of the new item.
 */
void TradeEventListener::itemAdded(const ItemContainer& container, const std::shared_ptr<Item>& item, size_t slot)
{
    // Inform our partner
    CharacterTradePartnerOfferItem offer;
    offer.slot     = slot;
    offer.type     = item->type();
    offer.typeId   = item->typeId();
    offer.quantity = item->quantity();
    partner_->session().write(offer);
}

/**
 * Gets executed when an item is removed from a container.
 * @param container The container instance.
 * @param item      The item that was removed.
 * @param slot      The slot the item was removed from.
 */
void TradeEventListener::itemRemoved(const ItemContainer& container, const std::shared_ptr<Item>& item, size_t slot)
{
    // Inform our partner
    partner_->session().write(CharacterTradePartnerOfferItem{ .slot = static_cast<uint8_t>(slot) });
}

/**
 * Gets executed when an item is transferred between containers.
 * @param source        The source container.
 * @param dest          The destination container
 * @param sourceSlot    The source slot
 * @param destSlot      The destination slot.
 */
void TradeEventListener::itemTransferred(const ItemContainer& source, const ItemContainer& dest, size_t sourceSlot,
                                         size_t destSlot)
{
}

/**
 * Synchronises the container with this listener.
 * @param container The container that was updated.
 */
void TradeEventListener::sync(const ItemContainer& container)
{
}