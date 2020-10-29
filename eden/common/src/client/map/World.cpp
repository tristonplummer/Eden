#include <shaiya/common/client/map/World.hpp>

#include <boost/filesystem.hpp>
#include <boost/throw_exception.hpp>
#include <glog/logging.h>

using namespace shaiya::client;

/**
 * Parses a client WLD file.
 * @param path  The path to the file.
 */
World::World(const std::string& path)
{
    // The path to the file
    if (!boost::filesystem::exists(path))
        BOOST_THROW_EXCEPTION(std::invalid_argument("File not found"));

    // The stream to the file
    std::ifstream stream(path.c_str(), std::ios::in | std::ios::binary);

    // Read the header of the WLD file.
    char header[4]{ 0 };
    stream.read(reinterpret_cast<char*>(&header), sizeof(header));

    // Parse a field file
    if (strcmp(header, "FLD") == 0)
    {
        parseField(stream);
    }
}

/**
 * Parses the world as a field.
 * @param stream    The stream to read from.
 */
void World::parseField(std::ifstream& stream)
{
    uint32_t size;
    stream.read(reinterpret_cast<char*>(&size), sizeof(size));

    // Set the size
    if (size == 4)
        size = 1024;
    else if (size == 8)
        size = 2048;

    // Parse the heightmap
    heightmap_ = Heightmap(stream, size);
}