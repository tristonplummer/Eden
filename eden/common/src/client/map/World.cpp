#include <shaiya/common/client/map/World.hpp>

#include <boost/filesystem.hpp>
#include <boost/throw_exception.hpp>
#include <glog/logging.h>

#include <cmath>

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
    stream.read(reinterpret_cast<char*>(&size_), sizeof(size_));

    // Set the size
    if (size_ == 4)
        size_ = 1024;
    else if (size_ == 8)
        size_ = 2048;

    // The size of the map
    sideSize_    = (size_ / 2) + 1;
    auto mapSize = sideSize_ * sideSize_;

    // Parse the heightmap
    heightmap_ = Heightmap(stream, size_);

    // Read the textures used at each point in the world
    textures_.resize(mapSize);
    stream.read(reinterpret_cast<char*>(textures_.data()), textures_.size());

    // Read the textures and sounds that should be loaded
    uint32_t textureAudioCount;
    stream.read(reinterpret_cast<char*>(&textureAudioCount), sizeof(textureAudioCount));
    for (auto i = 0; i < textureAudioCount; i++)
    {
        TextureAudio data{};
        stream.read(reinterpret_cast<char*>(&data), sizeof(data));
        textureAudios_.push_back(data);
    }

    // Continue parsing the file
    parseCommon(stream);
}

/**
 * Parses the common structure of both fields and dungeons.
 * @param stream    The stream to read from.
 */
void World::parseCommon(std::ifstream& stream)
{
    // Read the water texture file
    VariableString<256> waterTexture;
    stream.read(reinterpret_cast<char*>(&waterTexture), sizeof(waterTexture));
    waterTexture_ = waterTexture.str();

    parseModels(stream);
}

/**
 * Parses the models that are placed in the world.
 * @param stream    The stream to read from.
 */
void World::parseModels(std::ifstream& stream)
{
    // A helper function to parse the model maps
    auto read = [&]() -> std::map<std::string, std::vector<ShaiyaModelPosition>> {
        // The map of names to their model positions
        std::map<std::string, std::vector<ShaiyaModelPosition>> map{};

        // The vector of file names
        std::vector<std::string> names;

        // Read the number of file names
        uint32_t qty;
        stream.read(reinterpret_cast<char*>(&qty), sizeof(qty));

        // Read the file names
        names.resize(qty);
        for (auto i = 0; i < qty; i++)
        {
            VariableString<256> name;
            stream.read(reinterpret_cast<char*>(&name), sizeof(name));
            names.at(i) = name.str();
        }

        // Read the positions of the models
        uint32_t positionsQty;
        stream.read(reinterpret_cast<char*>(&positionsQty), sizeof(positionsQty));

        // Loop through the positions
        for (auto i = 0; i < positionsQty; i++)
        {
            // Read the position
            ShaiyaModelPosition position{};
            stream.read(reinterpret_cast<char*>(&position), sizeof(position));

            // The name of the model
            auto name = position.id > names.size() ? "" : names.at(position.id);
            if (map.contains(name))
            {
                auto& positions = map[name];
                positions.push_back(position);
            }
            else
            {
                std::vector<ShaiyaModelPosition> positions;
                positions.push_back(position);
                map[name] = positions;
            }
        }

        return map;
    };

    // Read the models
    buildings_     = read();
    shapes_        = read();  // Shapes are decorative models such as rocks, flags, statues, etc...
    trees_         = read();
    grass_         = read();
    primaryVani_   = read();  // VAni seems to be environmental animations, such as butterflies, grass movement, etc...
    secondaryVani_ = read();
    dungeons_      = read();
}

/**
 * Gets the texture at a specific point.
 * @param x The x coordinate
 * @param z The z coordinate
 * @return  The texture.
 */
[[nodiscard]] uint8_t World::textureAt(float x, float z) const
{
    auto posX = (uint32_t)std::floor(x);
    auto posZ = (uint32_t)std::floor(z);

    return textures_.at(posZ * sideSize_ + posX);
}