#pragma once
#include <shaiya/common/DataTypes.hpp>

#include <fstream>
#include <string>
#include <vector>

namespace shaiya::client
{
    /**
     * Represents the heightmap of a game map.
     */
    class Heightmap
    {
    public:
        /**
         * Default initialises the heightmap, which does nothing.
         */
        Heightmap() = default;

        /**
         * Loads a heightmap with a specific size.
         * @param path  The path to the heightmap file.
         * @param size  The size of the map.
         */
        explicit Heightmap(const std::string& path, size_t size);

        /**
         * Gets the y height at a specific set of coordinates.
         * @param x The x coordinate
         * @param z The z coordinate
         * @return  The floor height
         */
        [[nodiscard]] float y(float x, float z) const;

    private:
        /**
         * Parses the raw data.
         * @param stream    The file stream.
         */
        void parseRaw(std::ifstream& stream);

        /**
         * The size of the map.
         */
        size_t size_{ 0 };

        /**
         * The size of a side.
         */
        size_t sideSize_{ 0 };

        /**
         * The raw heightmap data.
         */
        std::vector<uint8_t> data_{};
    };

}