#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/client/svmap/MonsterArea.hpp>
#include <shaiya/common/client/svmap/NpcSpawn.hpp>

#include <vector>

namespace shaiya::client
{
    /**
     * Represents a parsed SVMap file.
     */
    class ServerMap
    {
    public:
        /**
         * Parses the server map file.
         * @param path  The path to the map file.
         */
        explicit ServerMap(const std::string& path);

        /**
         * Gets the size of the map.
         * @return  The size of the map.
         */
        [[nodiscard]] size_t size() const
        {
            return size_;
        }

        /**
         * Gets the ladder spawns for this map.
         * @return  The ladders.
         */
        [[nodiscard]] const std::vector<Shaiya3DVector>& ladders() const
        {
            return ladders_;
        }

        /**
         * Gets the monster spawns for this map.
         * @return  The monsters.
         */
        [[nodiscard]] const std::vector<MonsterArea>& monsters() const
        {
            return monsters_;
        }

        /**
         * Gets the npc spawns for this map.
         * @return  The npcs.
         */
        [[nodiscard]] const std::vector<NpcSpawn>& npcs() const
        {
            return npcs_;
        }

    private:
        /**
         * Parses the ladder spawns from the server map.
         * @param stream    The input stream.
         */
        void parseLadders(std::ifstream& stream);

        /**
         * Parses the monster spawns from the server map.
         * @param stream    The input stream.
         */
        void parseMonsters(std::ifstream& stream);

        /**
         * Parses the npc spawns from the server map.
         * @param stream    The input stream.
         */
        void parseNpcs(std::ifstream& stream);

        /**
         * The size of the map.
         */
        size_t size_{ 0 };

        /**
         * The ladder spawns.
         */
        std::vector<Shaiya3DVector> ladders_;

        /**
         * The monster spawn areas.
         */
        std::vector<MonsterArea> monsters_;

        /**
         * The npc spawns.
         */
        std::vector<NpcSpawn> npcs_;
    };
}