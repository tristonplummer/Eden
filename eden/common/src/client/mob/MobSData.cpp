#include <shaiya/common/client/mob/MobSData.hpp>

#include <boost/filesystem.hpp>

#include <fstream>

using namespace shaiya::client;

/**
 * Initialises this MobSData by reading mob entries from a file path
 * @param path  The path to the monster sdata.
 */
MobSData::MobSData(const std::string& path)
{
    if (!boost::filesystem::exists(path))
        throw std::invalid_argument("File doesn't exist.");

    // Open a stream to the item sdata file
    std::ifstream stream(path, std::ios::in | std::ios::binary);

    // Read the number of mob entries
    uint32_t total;
    stream.read(reinterpret_cast<char*>(&total), sizeof(total));

    // Loop through the mob entries
    for (auto id = 0; id < total; id++)
    {
        decode(stream, id);
    }
}

/**
 * Decodes the stream for an mob entry.
 * @param stream    The file stream.
 * @param type      The mob id.
 */
void MobSData::decode(std::ifstream& stream, size_t id)
{
    // A helper function to read a string that is prefixed by a 32-bit length
    auto readString = [&]() -> std::string {
        uint32_t length;
        stream.read(reinterpret_cast<char*>(&length), sizeof(length));

        std::string text;
        text.resize(length);
        stream.read(text.data(), length);
        return text;
    };

    // The mob definition
    auto def  = new MobDefinition{};
    def->id   = id;
    def->name = readString();

    // Read the mob definition
    stream.read(reinterpret_cast<char*>(&def->model), sizeof(MobDefinition) - offsetof(MobDefinition, model));
    stream.ignore(14);
    definitions_[id] = def;
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