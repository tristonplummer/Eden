#include <libpng/png.h>
#include <shaiya/common/client/map/Heightmap.hpp>

#include <boost/filesystem.hpp>
#include <boost/throw_exception.hpp>
#include <glog/logging.h>

#include <cmath>

using namespace shaiya::client;

/**
 * Loads a heightmap with a specific size.
 * @param path  The path to the heightmap file.
 * @param size  The size of the map.
 */
Heightmap::Heightmap(const std::string& path, size_t size): size_(size)
{
    if (!boost::filesystem::exists(path))
        BOOST_THROW_EXCEPTION(std::invalid_argument("File not found"));

    std::ifstream stream(path.c_str(), std::ios::in | std::ios::binary);

    if (path.ends_with(".data"))
        parseRaw(stream);
    else
        BOOST_THROW_EXCEPTION(std::invalid_argument("Only raw heightmap bytes are supported."));
}

/**
 * Parses the raw data.
 * @param stream    The file stream.
 */
void Heightmap::parseRaw(std::ifstream& stream)
{
    // Calculate the total size of the heightmap input
    sideSize_ = (size_ / 2) + 1;
    auto size = (sideSize_ * sideSize_) * 2;

    // Initialise the heightmap data
    data_.resize(size);

    // Read the data from the stream
    stream.read(reinterpret_cast<char*>(data_.data()), data_.size());
}

/**
 * Gets the y height at a specific set of coordinates.
 * @param x The x coordinate
 * @param z The z coordinate
 * @return  The floor height
 */
float Heightmap::y(float x, float z) const
{
    auto posX = (uint32_t)std::floor(x);
    auto posZ = (uint32_t)std::floor(z);

    auto idx = posZ / 2 * sideSize_ * 2 + posX / 2 * 2;

    auto value = *reinterpret_cast<uint16_t*>((uint16_t*)(data_.data() + idx));
    return (value - 10000) / 50.0;
}