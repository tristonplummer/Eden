#include <shaiya/common/client/svmap/ServerMap.hpp>

#include <boost/filesystem.hpp>
#include <boost/throw_exception.hpp>

#include <stdexcept>

using namespace shaiya::client;

/**
 * The size of a small map, or dungeon.
 */
constexpr auto SMALL_MAP_SIZE = 1024;

/**
 * The size of a large map.
 */
constexpr auto LARGE_MAP_SIZE = 2048;

/**
 * Parses the server map file.
 * @param path  The path to the map file.
 */
ServerMap::ServerMap(const std::string& path)
{
    if (!boost::filesystem::exists(path))
        BOOST_THROW_EXCEPTION(std::invalid_argument("File not found"));

    uint32_t size{};  // The size of the map.

    std::ifstream stream(path.c_str(), std::ios::in | std::ios::binary);
    stream.read(reinterpret_cast<char*>(&size), sizeof(size));

    // Set the map size
    if (size == 4)
        size_ = SMALL_MAP_SIZE;
    else if (size == 8)
        size_ = LARGE_MAP_SIZE;
    else
        size_ = size;

    // Skip the heightmap
    auto heightmapLength = (size_ * size_) / 8;
    stream.ignore(heightmapLength);
    stream.ignore(4);  // Skip an unknown int

    // Parse the map spawns
    parseLadders(stream);
    parseMonsters(stream);
    parseNpcs(stream);
}

/**
 * Parses the ladder spawns from the server map.
 * @param stream    The input stream.
 */
void ServerMap::parseLadders(std::ifstream& stream)
{
    uint32_t quantity;  // Read the number of ladders
    stream.read(reinterpret_cast<char*>(&quantity), sizeof(quantity));

    for (auto i = 0; i < quantity; i++)
    {
        Shaiya3DVector ladder{};
        stream.read(reinterpret_cast<char*>(&ladder), sizeof(ladder));

        ladders_.push_back(ladder);
    }
}

/**
 * Parses the monster spawns from the server map.
 * @param stream    The input stream.
 */
void ServerMap::parseMonsters(std::ifstream& stream)
{
    uint32_t quantity;  // Read the number of monster areas
    stream.read(reinterpret_cast<char*>(&quantity), sizeof(quantity));

    for (auto i = 0; i < quantity; i++)
    {
        MonsterArea area{};
        auto length = sizeof(area.bottomLeft) + sizeof(area.topRight) + sizeof(area.recordQty);
        stream.read(reinterpret_cast<char*>(&area), length);

        for (auto j = 0; j < area.recordQty; j++)
        {
            MonsterSpawn spawn{};
            stream.read(reinterpret_cast<char*>(&spawn), sizeof(spawn));

            area.spawns.push_back(spawn);
        }

        monsters_.push_back(area);
    }
}

/**
 * Parses the npc spawns from the server map.
 * @param stream    The input stream.
 */
void ServerMap::parseNpcs(std::ifstream& stream)
{
    uint32_t quantity;  // Read the number of npc spawns.
    stream.read(reinterpret_cast<char*>(&quantity), sizeof(quantity));

    for (auto i = 0; i < quantity; i++)
    {
        NpcSpawn spawn{};
        auto length = sizeof(spawn.type) + sizeof(spawn.typeId) + sizeof(spawn.quantity);
        stream.read(reinterpret_cast<char*>(&spawn), length);

        for (auto j = 0; j < spawn.quantity; j++)
        {
            NpcSpawnPosition pos{};
            stream.read(reinterpret_cast<char*>(&pos), sizeof(pos));

            spawn.positions.push_back(pos);
        }

        npcs_.push_back(spawn);
    }
}