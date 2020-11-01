#include <shaiya/common/client/mob/MobSData.hpp>

using namespace shaiya::client;

/**
 * Initialises this MobSData by reading mob entries from a file path
 * @param path  The path to the monster sdata.
 */
MobSData::MobSData(const std::string& path)
{
}

/**
 * Decodes the stream for an mob entry.
 * @param stream    The file stream.
 * @param type      The mob id.
 */
void MobSData::decode(std::ifstream& stream, size_t id)
{
}

/**
 * Gets the mob definition for a given id.
 * @param id    The mob id.
 * @return      The mob definition.
 */
const MobDefinition* MobSData::forId(uint32_t id) const
{
    if (!definitions_.contains(id))
        return nullptr;
    return definitions_.at(id);
}