#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/CharacterScreenService.hpp>
#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/service/ServiceContext.hpp>

#include <regex>

using namespace shaiya;
using namespace shaiya::net;

/**
 * The regex pattern for validating names against
 */
constexpr auto CHARACTER_NAME_PATTERN = "^[a-zA-Z0-9]*$";

/**
 * Handles an incoming character creation request.
 * @param session   The session instance.
 * @param request   The inbound character creation request.
 */
void handleCharacterCreate(Session& session, const CharacterCreationRequest& request)
{
    auto& game       = dynamic_cast<GameSession&>(session);
    auto& charScreen = game.context().getCharScreen();

    // A helper function to send a result
    auto sendResult = [&](CharacterCreateResult result) {
        CharacterCreationResponse response;
        response.result = result;
        game.write(response);
    };

    // Validate the character details
    if (request.race > ShaiyaRace::Nordein)
        return sendResult(CharacterCreateResult::Error);
    if (request.job > ShaiyaClass::Priest)
        return sendResult(CharacterCreateResult::Error);
    if (request.mode > ShaiyaGameMode::Ultimate)
        return sendResult(CharacterCreateResult::Error);
    if (request.gender > ShaiyaGender::Female)
        return sendResult(CharacterCreateResult::Error);

    // Ensure that the race and class matches the session's faction
    auto faction = game.faction();
    if (faction == ShaiyaFaction::Fury)  // Fury Faction Validation
    {
        // Ensure that a Fury player is creating a Fury race.
        if (request.race != ShaiyaRace::Vail && request.race != ShaiyaRace::Nordein)
            return sendResult(CharacterCreateResult::Error);

        if (request.race == ShaiyaRace::Vail)  // Only Pagan, Oracle and Assassin can be Vail
        {
            if (request.job != ShaiyaClass::Mage && request.job != ShaiyaClass::Priest && request.job != ShaiyaClass::Ranger)
                return sendResult(CharacterCreateResult::Error);
        }

        if (request.race == ShaiyaRace::Nordein)  // Only Warrior, Guardian and Hunter can be Nordein
        {
            if (request.job != ShaiyaClass::Fighter && request.job != ShaiyaClass::Defender &&
                request.job != ShaiyaClass::Archer)
                return sendResult(CharacterCreateResult::Error);
        }
    }
    else if (faction == ShaiyaFaction::Light)  // Light Faction Validation
    {
        // Ensure that a Light player is creating a Light race.
        if (request.race != ShaiyaRace::Human && request.race != ShaiyaRace::Elf)
            return sendResult(CharacterCreateResult::Error);

        if (request.race == ShaiyaRace::Human)  // Only Fighter, Defender and Priest can be Human
        {
            if (request.job != ShaiyaClass::Fighter && request.job != ShaiyaClass::Defender &&
                request.job != ShaiyaClass::Priest)
                return sendResult(CharacterCreateResult::Error);
        }

        if (request.race == ShaiyaRace::Elf)  // Only Ranger, Archer and Mage can be Elf
        {
            if (request.job != ShaiyaClass::Ranger && request.job != ShaiyaClass::Archer && request.job != ShaiyaClass::Mage)
                return sendResult(CharacterCreateResult::Error);
        }
    }
    else if (faction == ShaiyaFaction::Neither)  // Players who haven't chosen a faction can't create characters.
    {
        return game.close();
    }

    // Validate the username
    if (!std::regex_match(request.name.str(), std::regex(CHARACTER_NAME_PATTERN)))
        return sendResult(CharacterCreateResult::NameNotAvailable);

    // Prepare the request
    auto slot   = request.slot;
    auto race   = (int)request.race;
    auto mode   = (int)request.mode;
    auto hair   = request.hair;
    auto face   = request.face;
    auto height = request.height;
    auto job    = (int)request.job;
    auto gender = (int)request.gender;
    auto name   = request.name.str();

    // Process the character creation request
    auto result = charScreen.createCharacter(game, slot, race, mode, hair, face, height, job, gender, name);
    if (result != CharacterCreateResult::Success)
        return sendResult(result);

    // If the character creation was successful, we should also re-send the character screen
    sendResult(CharacterCreateResult::Success);
    charScreen.display(game);
}

/**
 * A template specialization used for registering a character creation request handler.
 */
template<>
void PacketRegistry::registerPacketHandler<CharacterCreateOpcode>()
{
    registerHandler<CharacterCreateOpcode, CharacterCreationRequest>(&handleCharacterCreate, ExecutionType::Asynchronous);
}