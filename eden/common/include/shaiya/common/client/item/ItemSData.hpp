#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/client/item/ItemDefinition.hpp>

#include <fstream>
#include <map>

namespace shaiya::client
{
    /**
     * Represents a decoded Item.SData file from the client. This is used for parsing and retrieving item definitions from
     * the client.
     */
    class ItemSData
    {
    public:
        /**
         * Default constructor, does nothing.
         */
        ItemSData() = default;

        /**
         * Initialises this ItemSData by reading item entries from a file path
         * @param path  The path to the item sdata.
         */
        explicit ItemSData(const std::string& path);

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
         * Decodes the stream for an item entry.
         * @param stream    The file stream.
         * @param type      The item type.
         * @param typeId    The item type id.
         */
        void decode(std::ifstream& stream, size_t type, size_t typeId);

        /**
         * A map of item ids, to their item definitions.
         */
        std::map<uint32_t, ItemDefinition*> definitions_;
    };
}