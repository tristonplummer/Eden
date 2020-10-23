#include <shaiya/common/net/packet/game/CharacterActionBar.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Initialise the action bar for a character.
 * @param player The character instance.
 */
ActionBar::ActionBar(Player& player): player_(player)
{
}

/**
 * Informs the character about the state of their action bar.
 */
void ActionBar::sync()
{
    // Send an empty action bar, for now.
    player_.session().write(CharacterActionBarUpdate{});
}