#pragma once
#include <shaiya/common/client/mob/MobDefinition.hpp>
#include <shaiya/common/net/packet/game/MobState.hpp>
#include <shaiya/game/model/Area.hpp>
#include <shaiya/game/model/actor/Actor.hpp>

namespace shaiya::game
{
    /**
     * Represents a monster in the game world. Mobs are generally (but not always) aggressive towards all players. A mob can
     * be thought of as an npc that may be attacked by players - unlike npcs, who may attack players but may not be attacked.
     *
     * The keen eyed among you may think, "but Cups, technically a mob is what we call an actor". Look, I know. I'm trying to
     * stay relatively consistent with the naming Shaiya uses. They use the term "Monster" in the SData, but all of the
     * monster definitions are labelled as "mob_wolf_...". Whatever, you all know what I mean here. There's no need to be
     * pedantic, ok?
     */
    class Mob: public Actor
    {
    public:
        /**
         * Initialises a mob with a specified definition.
         * @param def       The mob definition.
         * @param spawnArea The area that this mob can spawn in.
         * @param world     The game world instance.
         */
        Mob(const client::MobDefinition& def, Area spawnArea, GameWorldService& world);

        /**
         * Activates this actor.
         */
        void activate() override;

        /**
         * Processes the tick for this entity.
         */
        void tick() override;

        /**
         * Sets the position of this mob.
         * @param position  The new position.
         */
        void setPosition(Position position) override;

        /**
         * Gets the definition for this mob.
         * @return  The definition.
         */
        [[nodiscard]] const client::MobDefinition& definition() const
        {
            return def_;
        }

        /**
         * Gets the spawn area of this mob.
         * @return  The spawn area.
         */
        [[nodiscard]] const Area& spawnArea() const
        {
            return spawnArea_;
        }

        /**
         * Gets the state of this mob.
         * @return  The mob state.
         */
        [[nodiscard]] shaiya::net::MobState state() const
        {
            shaiya::net::MobState state;
            state.id        = id();
            state.hitpoints = stats_.currentHitpoints();
            return state;
        }

    private:
        /**
         * The definition of this mob.
         */
        const client::MobDefinition& def_;

        /**
         * The area that this mob can spawn in.
         */
        Area spawnArea_;
    };
}