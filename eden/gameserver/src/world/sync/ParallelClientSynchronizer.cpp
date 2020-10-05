#include <shaiya/game/world/model/actor/character/Character.hpp>
#include <shaiya/game/world/sync/ParallelClientSynchronizer.hpp>
#include <shaiya/game/world/sync/task/CharacterSynchronizationTask.hpp>

#include <execution>

using namespace shaiya::game;

/**
 * Synchronizes the state of the clients with the stat of the server.
 * @param characters    The vector containing the player characters.
 */
void ParallelClientSynchronizer::synchronize(std::vector<std::shared_ptr<Character>> characters)
{
    // Execute the character synchronization task for all characters
    auto characterTask = [](const std::shared_ptr<Character>& character) {
        auto& player = *character;
        if (player.active())
        {
            CharacterSynchronizationTask task(player);
            task.sync();
        }
    };
    std::for_each(std::execution::par_unseq, characters.begin(), characters.end(), characterTask);
}