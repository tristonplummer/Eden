#include <shaiya/common/net/packet/game/PlayerAddExperience.hpp>
#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Initialises the level of this player.
 * @param player    The player.
 */
Level::Level(Player& player): player_(player), world_(player_.world())
{
}

/**
 * Initialises the level data for a player.
 * @param mode          The game mode of the player.
 * @param level         The player's current level.
 * @param experience    The player's current experience.
 */
void Level::init(ShaiyaGameMode mode, uint16_t level, uint16_t experience)
{
    mode_       = mode;
    level_      = level;
    experience_ = experience;

    auto [maxLevel, statpoints, skillpoints] = world_.getPlayerLevelDefinition(mode_);
    maxLevel_                                = maxLevel;
    baseExperience_                          = world_.getTotalExperience(mode_, level_ - 1);
    requiredExperience_                      = world_.getTotalExperience(mode_, level_);
}

/**
 * Sets the experience of a player.
 * @param experience    The new experience value.
 */
bool Level::setExperience(size_t experience)
{
    if (level_ >= maxLevel_)
        return false;
    experience_ = experience;

    if (experience_ >= requiredExperience_)
        setLevel(level_ + 1, false);

    return true;
}

/**
 * Adds experience to the player.
 * @param experience    The experience to add.
 */
void Level::addExperience(size_t experience)
{
    if (setExperience(experience_ + experience))
    {
        PlayerAddExperience add;
        add.experience = experience;
        player_.session().write(add);
    }
}

/**
 * Sets the level of the player.
 * @param level             The new level.
 * @param adjustExperience  Adjust the player's experience to match the new level.
 */
void Level::setLevel(uint16_t level, bool adjustExperience)
{
    auto [_, statpoints, skillpoints] = world_.getPlayerLevelDefinition(mode_);

    if (level > maxLevel_)
        level = maxLevel_;

    auto prevLevel = level_;
    level_         = level;

    if (level_ > prevLevel)
    {
        auto delta = (level_ - prevLevel);

        auto additionalStatpoints  = (statpoints * delta);
        auto additionalSkillpoints = (skillpoints * delta);

        player_.setStatpoints(player_.statpoints() + additionalStatpoints);
        player_.setSkillpoints(player_.skillpoints() + additionalSkillpoints);
    }

    baseExperience_     = world_.getTotalExperience(mode_, level_ - 1);
    requiredExperience_ = world_.getTotalExperience(mode_, level_);

    if (adjustExperience)
    {
        experience_ = world_.getTotalExperience(mode_, level_ - 1);
    }

    // Set the base definition values
    auto [hitpoints, mana, stamina] = player_.world().getBasePlayerDefinition(player_.job(), player_.level());
    auto& stats                     = player_.stats();
    stats.setBase(Stat::MaxHealth, hitpoints);
    stats.setBase(Stat::MaxMana, mana);
    stats.setBase(Stat::MaxStamina, stamina);
    player_.syncStats();

    // Bring the player to full health/mana/stamina
    stats.setHitpoints(stats.getTotal(Stat::MaxHealth));
    stats.setMana(stats.getTotal(Stat::MaxMana));
    stats.setStamina(stats.getTotal(Stat::MaxStamina));

    player_.flagUpdate(UpdateFlag::LevelUp);
}
