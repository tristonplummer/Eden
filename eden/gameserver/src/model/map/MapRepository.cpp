#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/model/map/MapRepository.hpp>

#include <boost/filesystem.hpp>
#include <glog/logging.h>

#include <cassert>

using namespace shaiya::game;

/**
 * Loads the map repository.
 * @param mapPath  The path to the world's map files.
 */
void MapRepository::load(const std::string& mapPath)
{
    using namespace boost::filesystem;
    path p(mapPath);  // The path to the map files

    // A helper function to get a map id for it's path
    auto idForPath = [](const path& file) -> int {
        auto name = file.filename().string();
        return std::stoi(name.substr(0, name.find(".wld")));
    };

    // A vector of file paths
    std::vector<path> paths;
    std::copy(directory_iterator(p), directory_iterator(), std::back_inserter(paths));
    std::sort(paths.begin(), paths.end(), [&](const path& lhs, const path& rhs) { return idForPath(lhs) < idForPath(rhs); });

    // Resize the vector to fit all the maps by their id
    auto lastId = idForPath(paths.back());
    maps_.resize(lastId + 1);

    // Load all of the maps
    for (auto&& mapFilePath: paths)
    {
        // The stream to the map file
        std::ifstream stream(mapFilePath.string(), std::ios::in | std::ios::binary | std::ios::ate);
        auto size = stream.tellg();
        stream.seekg(std::ios::beg);

        // Create and parse the map file
        auto map = std::make_shared<Map>();
        map->load(stream, size);

        // Store the map
        auto id      = idForPath(mapFilePath);
        maps_.at(id) = std::move(map);
    }
}

/**
 * Gets a map for a specified id.
 * @param id    The map id.
 * @return      The map instance.
 */
std::shared_ptr<Map> MapRepository::forId(uint16_t id) const
{
    assert(maps_.size() > id);
    return maps_.at(id);
}