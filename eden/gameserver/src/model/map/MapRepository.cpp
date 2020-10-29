#include <shaiya/common/client/map/Heightmap.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/model/map/MapRepository.hpp>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <glog/logging.h>

using namespace shaiya::game;

/**
 * Loads the map repository.
 * @param mapPath  The path to the world's map files.
 * @param world     The game world service.
 */
void MapRepository::load(const std::string& mapPath, GameWorldService& world)
{
    using namespace boost::filesystem;
    path p(mapPath);  // The path to the map files

    // Loop over all the directories in the map path
    for (auto& directory: boost::make_iterator_range(directory_iterator(p), {}))
    {
        auto getPath = [&](const std::string& path) {
            auto p = directory.path();
            return p /= path;
        };
        auto metadata  = getPath("map.yaml");
        auto heightmap = getPath("heightmap.data");

        if (!exists(metadata))  // If the map metadata file doesn't exist, skip this directory.
            continue;

        // Initialise the map and parse the metadata file
        std::ifstream metastream(metadata.c_str(), std::ios::in);
        auto map = std::make_shared<Map>(world);
        map->load(metastream);

        // Store the map
        maps_[map->id()] = map;

        // Load the heightmap
        map->loadHeightmap(heightmap.string());

        // Loop over the npc spawns
        for (auto& npc: boost::make_iterator_range(directory_iterator(getPath("/npcs")), {}))
        {
            std::ifstream stream(npc.path().c_str(), std::ios::in);
            map->loadNpc(stream);
        }
    }
}

/**
 * Gets a map for a specified id.
 * @param id    The map id.
 * @return      The map instance.
 */
std::shared_ptr<Map> MapRepository::forId(uint16_t id) const
{
    if (maps_.count(id))
        return maps_.at(id);
    return nullptr;
}