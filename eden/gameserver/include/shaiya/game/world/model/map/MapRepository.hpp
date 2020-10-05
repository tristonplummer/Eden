#pragma once
#include <shaiya/game/world/model/map/Map.hpp>

#include <map>
#include <memory>
#include <vector>

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
         */
        void load(const std::string& path);

        /**
         * Gets a map for a specified id.
         * @param id    The map id.
         * @return      The map instance.
         */
        [[nodiscard]] std::shared_ptr<Map> forId(uint16_t id) const;

    private:
        /**
         * The vector of maps.
         */
        std::vector<std::shared_ptr<Map>> maps_;
    };
}