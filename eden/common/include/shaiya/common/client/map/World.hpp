#pragma once
#include <shaiya/common/client/map/Heightmap.hpp>

#include <fstream>

namespace shaiya::client
{
    /**
     * Handles the parsing of a Shaiya world file.
     */
    class World
    {
    public:
        /**
         * Parses a client WLD file.
         * @param path  The path to the file.
         */
        explicit World(const std::string& path);

    private:
        /**
         * Parses the world as a field.
         * @param stream    The stream to read from.
         */
        void parseField(std::ifstream& stream);

        /**
         * The heightmap of this world.
         */
        Heightmap heightmap_;
    };
}