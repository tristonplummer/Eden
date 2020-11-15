#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/commands/CommandManager.hpp>
#include <shaiya/game/model/commands/impl/HeightmapCommand.hpp>
#include <shaiya/game/model/commands/impl/MoveMobCommand.hpp>
#include <shaiya/game/model/commands/impl/MoveNpcCommand.hpp>
#include <shaiya/game/model/commands/impl/SpawnItemCommand.hpp>
#include <shaiya/game/model/commands/impl/SpawnMobCommand.hpp>
#include <shaiya/game/model/commands/impl/SpawnNpcCommand.hpp>
#include <shaiya/game/model/commands/impl/TeleportCommand.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <glog/logging.h>

using namespace shaiya::game;

/**
 * Initialises the command manager.
 */
CommandManager::CommandManager()
{
    registerCommand(std::make_shared<SpawnItemCommand>());
    registerCommand(std::make_shared<TeleportCommand>());
    registerCommand(std::make_shared<SpawnNpcCommand>());
    registerCommand(std::make_shared<SpawnMobCommand>());
    registerCommand(std::make_shared<MoveNpcCommand>());
    registerCommand(std::make_shared<HeightmapCommand>());
    registerCommand(std::make_shared<MoveMobCommand>());
}

/**
 * Attempts to execute a command.
 * @param player The character that is trying to execute a command.
 * @param text      The command text.
 */
void CommandManager::execute(Player& player, const std::string& text) const
{
    using namespace std;
    using namespace boost;

    try
    {
        // Split the string by spaces, unless they are surrounded in quotation marks.
        vector<string> args;
        tokenizer<escaped_list_separator<char>> token(text, escaped_list_separator<char>('\\', ' ', '\"'));
        for (auto itr = token.begin(); itr != token.end(); ++itr)
            args.push_back(*itr);

        // Erase the first argument, as it's our identifier
        auto identifier = (*args.begin()).substr(1);
        args.erase(args.begin());
        algorithm::to_lower(identifier);

        // If a command with the specified identifier exists, execute it.
        if (commands_.contains(identifier))
        {
            auto command = commands_.at(identifier);
            command->execute(player, args);
        }
    }
    catch (const std::exception& e)
    {
        LOG(INFO) << "Command executed by \"" << player.name() << "\" with text: \"" << text
                  << "\" threw an exception: " << e.what();
    }
}

/**
 * Registers a command instance.
 * @param command   The command instance.
 */
void CommandManager::registerCommand(std::shared_ptr<Command> command)
{
    commands_[command->identifier()] = std::move(command);
}