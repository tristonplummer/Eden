#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/client/mob/MobDefinition.hpp>

#include <fstream>
#include <map>

namespace shaiya::client
{
    /**
     * Represents a decoded Monster.SData file from the client. This is used for parsing and retrieving mob definitions from
     * the client.
     */
    class MobSData
    {
    public:
        /**
         * Default constructor, does nothing.
         */
        MobSData() = default;

        /**
         * Initialises this MobSData by reading mob entries from a file path
         * @param path  The path to the monster sdata.
         */
        explicit MobSData(const std::string& path);

        /**
         * Gets the mob definition for a given id.
         * @param id    The mob id.
         * @return      The mob definition.
         */
        [[nodiscard]] const MobDefinition* forId(uint32_t id) const;

    private:
        /**
         * Decodes the stream for an mob entry.
         * @param stream    The file stream.
         * @param type      The mob id.
         */
        void decode(std::ifstream& stream, size_t id);

        /**
         * A map of mob ids, to their mob definitions.
         */
        std::map<uint32_t, MobDefinition*> definitions_;
    };
}