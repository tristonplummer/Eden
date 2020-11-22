#include <shaiya/game/model/actor/Actor.hpp>
#include <shaiya/game/scheduling/impl/HealthNormalizationTask.hpp>

#include <cmath>

using namespace shaiya::game;

/**
 * The delay between normalizing the health of an actor. This is set to 60 as we want to run this task every 3 seconds.
 * 3 seconds = 3000ms
 * 1 pulse  = 50ms
 * 60 * 50ms = 3 seconds
 */
constexpr auto NormalizationDelay = 60;

/**
 * The restoration value, represented as a decimal.
 * 0.03 = 3%
 */
constexpr auto NaturalRestorationValue = 0.03;

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
void HealthNormalizationTask::execute(GameWorldService& world)
{
    // If the actor is not active, stop this task
    if (!actor_.active())
        return stop();
    if (actor_.dead())
        return;
    if (actor_.type() != EntityType::Player)
        return;

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
    auto maxStamina     = stats.maxStamina();

    // Normalize the current health
    currentHealth += std::ceil(maxHealth * NaturalRestorationValue);
    stats.setHitpoints(currentHealth);

    // Normalize the current mana
    currentMana += std::ceil(maxMana * NaturalRestorationValue);
    stats.setMana(currentMana);

    // Normalize the current stamina
    currentStamina += std::ceil(maxStamina * NaturalRestorationValue);
    stats.setStamina(currentStamina);
}