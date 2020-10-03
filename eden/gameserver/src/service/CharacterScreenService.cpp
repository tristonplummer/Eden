#include <shaiya/game/net/GameSession.hpp>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Initialises the character screen service.
 * @param db    The database service to use.
 */
CharacterScreenService::CharacterScreenService(shaiya::database::DatabaseService& db): db_(db)
{
}

/**
 * Displays the character screen for a session.
 * @param session   The session.
 */
void CharacterScreenService::display(GameSession& session)
{
    // Send the player their faction
    AccountFactionNotify faction;
    session.write(faction);

    // Send the empty character list
    for (auto i = 0; i < 5; i++)
    {
        CharacterListEntry entry;
        entry.slot = i;
        session.write(entry);
    }
}