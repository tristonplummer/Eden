#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/world/model/item/ItemDefinition.hpp>

#include <map>

namespace shaiya::game
{
    /**
     * A service that loads item definitions from a database, and provides an interface for accessing them.
     */
    class ItemDefinitionService
    {
    public:
        /**
         * Initialise this item definition service.
         * @param db    The database instance.
         */
        explicit ItemDefinitionService(shaiya::database::DatabaseService& db);

        /**
         * Gets the item definition for a given id.
         * @param id    The item id.
         * @return      The item definition.
         */
        [[nodiscard]] const ItemDefinition* forId(uint32_t id) const;

        /**
         * Gets the item definition for a given id.
         * @param type      The item type.
         * @param typeId    The item type id.
         * @return          The item definition.
         */
        [[nodiscard]] const ItemDefinition* forId(uint8_t type, uint8_t typeId) const;

    private:
        /**
         * A map of item ids, to their item definitions.
         */
        std::map<uint32_t, ItemDefinition*> definitions_;
    };
}