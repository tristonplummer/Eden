#pragma once
#include <shaiya/common/client/map/Heightmap.hpp>
#include <shaiya/common/client/map/ShaiyaModelPosition.hpp>
#include <shaiya/common/client/map/TextureAudio.hpp>

#include <fstream>
#include <map>
#include <vector>

namespace shaiya::client
{
    typedef std::map<std::string, std::vector<ShaiyaModelPosition>> ShaiyaModelMap;

    /**
     * Handles the parsing of a Shaiya world file.
     */
    class World
    {
    public:
        /**
         * The default constructor, which does nothing.
         */
        World() = default;

        /**
         * Parses a client WLD file.
         * @param path  The path to the file.
         */
        explicit World(const std::string& path);

        /**
         * Gets the heightmap for this world.
         * @return  The heightmap.
         */
        [[nodiscard]] const Heightmap& heightmap() const
        {
            return heightmap_;
        }

        /**
         * Gets the texture at a specific point.
         * @param x The x coordinate
         * @param z The z coordinate
         * @return  The texture.
         */
        [[nodiscard]] uint8_t textureAt(float x, float z) const;

    private:
        /**
         * Parses the world as a field.
         * @param stream    The stream to read from.
         */
        void parseField(std::ifstream& stream);

        /**
         * Parses the common structure of both fields and dungeons.
         * @param stream    The stream to read from.
         */
        void parseCommon(std::ifstream& stream);

        /**
         * Parses the models that are placed in the world.
         * @param stream    The stream to read from.
         */
        void parseModels(std::ifstream& stream);

        /**
         * The size of the map.
         */
        uint32_t size_{};

        /**
         * The size of a side of the map.
         */
        uint32_t sideSize_{};

        /**
         * The heightmap of this world.
         */
        Heightmap heightmap_;

        /**
         * The textures of the world.
         */
        std::vector<uint8_t> textures_;

        /**
         * A vector containing the names of the textures and sounds that are used in the world.
         */
        std::vector<TextureAudio> textureAudios_;

        /**
         * The name of the water texture file to use in this map.
         */
        std::string waterTexture_;

        /**
         * The buildings
         */
        ShaiyaModelMap buildings_;

        /**
         * The shapes
         */
        ShaiyaModelMap shapes_;

        /**
         * The trees
         */
        ShaiyaModelMap trees_;

        /**
         * The grass
         */
        ShaiyaModelMap grass_;

        /**
         * The environmental effects
         */
        ShaiyaModelMap primaryVani_;

        /**
         * The environmental effects
         */
        ShaiyaModelMap secondaryVani_;

        /**
         * The dungeons
         */
        ShaiyaModelMap dungeons_;
    };
}