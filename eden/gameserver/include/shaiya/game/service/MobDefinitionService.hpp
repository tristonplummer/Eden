#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/db/DatabaseService.hpp>
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/model/actor/mob/MobDefinition.hpp>

namespace shaiya::game
{
    /**
     * A utility class which handles the loading and retrieving of mob definitions.
     */
    class MobDefinitionService
    {
    public:
        /**
         * Initialises the mob definition service.
         * @param db    The database service.
         */
        explicit MobDefinitionService(shaiya::database::DatabaseService& db);

        /**
         * Gets the mob definition for a given id.
         * @param id    The mob id.
         * @return      The mob definition.
         */
        [[nodiscard]] const MobDefinition* forId(uint32_t id) const;

    private:
        /**
         * A map of mob ids, to their mob definitions.
         */
        std::map<uint32_t, MobDefinition*> definitions_;
    };
}