#include <shaiya/game/model/actor/Actor.hpp>
#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/scheduling/impl/ActorDeathTask.hpp>
#include <shaiya/game/scheduling/impl/MobRespawnTask.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

#include <glog/logging.h>

using namespace shaiya::game;

/**
 * Initialise this task.
 * @param actor The actor to operate on.
 */
ActorDeathTask::ActorDeathTask(Actor& actor): actor_(actor), ScheduledTask(1)
{
}

/**
 * Handle the execution of this task.
 */
void ActorDeathTask::execute(GameWorldService& world)
{
    // Flag the actor as dead
    actor_.flagUpdate(UpdateFlag::Death);

    // If the actor is a mob, submit a respawn event
    if (actor_.type() == EntityType::Mob)
    {
        Mob& mob  = dynamic_cast<Mob&>(actor_);
        auto& pos = mob.position();
        auto map  = mob.map();

        auto player = map->get<Player>(pos, 2, EntityType::Player);
        player->levelling().addExperience(3);

        world.schedule(std::make_shared<MobRespawnTask>(mob, 200));
    }

    stop();
}