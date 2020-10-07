#include <shaiya/game/world/model/actor/Actor.hpp>
#include <shaiya/game/world/scheduling/impl/HealthNormalizationTask.hpp>

using namespace shaiya::game;

/**
 * The delay between normalizing the health of an actor. This is set to 60 as we want to run this task every 3 seconds.
 * 3 seconds = 3000ms
 * 1 pulse  = 50ms
 * 60 * 50ms = 3 seconds
 */
constexpr auto NormalizationDelay = 60;

/**
 * The amount of HP/MP/SP to restore per execution, represented as a whole number.
 * The value 3 represents 3%.
 */
constexpr auto NaturalRestorationPercent = 3;

/**
 * The restoration value, represented as a decimal.
 */
constexpr auto NaturalRestorationValue = NaturalRestorationPercent / 100;

/**
 * Initialise this task.
 * @param actor The actor to operate on.
 */
HealthNormalizationTask::HealthNormalizationTask(Actor& actor): actor_(actor), ScheduledTask(NormalizationDelay)
{
}

/**
 * Handle the execution of this task.
 */
void HealthNormalizationTask::execute()
{
    // The actor's stats
    auto& stats = actor_.stats();

    // The current and maximum health values of the actor
    auto currentHealth = stats.currentHitpoints();
    auto maxHealth     = stats.maxHitpoints();

    // The current and maximum mana values of the actor
    auto currentMana = stats.currentMana();
    auto maxMana     = stats.maxMana();

    // The current and maximum stamina values of the actor
    auto currentStamina = stats.currentStamina();
    auto maxStamina     = stats.currentStamina();

    // Normalize the current health
    currentHealth += std::floor(maxHealth * NaturalRestorationValue);
    stats.setHitpoints(currentHealth);

    // Normalize the current mana
    currentMana += std::floor(maxMana * NaturalRestorationValue);
    stats.setMana(currentMana);

    // Normalize the current stamina
    currentStamina += std::floor(maxStamina * NaturalRestorationValue);
    stats.setStamina(currentStamina);
}