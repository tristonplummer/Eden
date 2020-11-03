#pragma once
#include <shaiya/game/Forward.hpp>

#include <map>
#include <memory>

namespace shaiya::game
{
    /**
     * A repository of maps, stored in a vector at an index that corresponds to their map id.
     */
    class MapRepository
    {
    public:
        /**
         * Loads the map repository.
         * @param path  The path to the world's map files.
         * @param world The game world service.
         */
        void load(const std::string& path, GameWorldService& world);

        /**
         * Processes the tick for all maps.
         */
        void tick();

        /**
         * Gets a map for a specified id.
         * @param id    The map id.
         * @return      The map instance.
         */
        [[nodiscard]] std::shared_ptr<Map> forId(uint16_t id) const;

        /**
         * Gets a vector of each map.
         * @return  The maps
         */
        [[nodiscard]] std::vector<std::shared_ptr<Map>> maps() const
        {
            std::vector<std::shared_ptr<Map>> maps;
            for (auto [id, map]: maps_)
                maps.push_back(map);
            return maps;
        }

    private:
        /**
         * The map of maps.
         */
        std::map<uint16_t, std::shared_ptr<Map>> maps_;
    };
}