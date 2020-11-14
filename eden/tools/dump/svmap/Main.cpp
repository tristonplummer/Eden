#include <shaiya/common/client/svmap/ServerMap.hpp>

#include <glog/logging.h>

#include <fstream>
#include <iostream>
#include <yaml-cpp/yaml.h>

/**
 * Converts the npc spawns.
 * @param map   The map.
 * @param out   The output stream.
 */
void convertNpcs(shaiya::client::ServerMap& map, std::ofstream& out);

/**
 * Converts the mob spawns.
 * @param map   The map.
 * @param out   The output stream.
 */
void convertMobs(shaiya::client::ServerMap& map, std::ofstream& out);

/**
 * The entry point for the servermap conversion tool.
 * @param argc  The number of command-line arguments.
 * @param argv  The command-line values.
 * @return      The status code.
 */
int main(int argc, char** argv)
{
    using namespace shaiya::client;
    if (argc != 3)
    {
        LOG(ERROR) << "Please provide the path to a SVMap file and an output file.";
        return 1;
    }

    std::ofstream out(argv[2], std::ios::out);

    ServerMap map(argv[1]);
    convertNpcs(map, out);
    convertMobs(map, out);

    out.close();
    return 0;
}

/**
 * Converts the npc spawns.
 * @param map   The map.
 * @param file  The output stream.
 */
void convertNpcs(shaiya::client::ServerMap& map, std::ofstream& file)
{
    using namespace YAML;
    Emitter out;

    out << BeginMap;
    out << Key << "npcs" << Value;
    out << BeginSeq;

    auto& npcs = map.npcs();
    for (auto&& npc: npcs)
    {
        // Start the npc spawn
        out << BeginMap << Key << "npc" << Value;
        out << BeginMap;

        out << Key << "type" << Value << (int)npc.type;
        out << Key << "typeid" << Value << (int)npc.typeId;
        out << Key << "positions" << Value << BeginSeq << Block;
        for (auto i = 0; i < npc.quantity; i++)
        {
            auto& pos = npc.positions[i];
            auto vec  = pos.position;
            out << Flow << BeginMap;
            out << Key << "x" << vec.x;
            out << Key << "y" << vec.y;
            out << Key << "z" << vec.z;
            out << Key << "dir" << pos.dir;
            out << EndMap << Block;
        }
        out << EndSeq;

        // End the spawn
        out << EndMap;
        out << EndMap;
    }

    std::cout << out.c_str();
}

/**
 * Converts the mob spawns.
 * @param map   The map.
 * @param out   The output stream.
 */
void convertMobs(shaiya::client::ServerMap& map, std::ofstream& file)
{
    using namespace YAML;
    Emitter out;

    out << BeginMap;
    out << Key << "mobs" << Value;
    out << BeginSeq;

    auto& spawns = map.monsters();
    for (auto&& spawn: spawns)
    {
        // Start the mob spawn
        out << BeginMap << Key << "spawn" << Value;
        out << BeginMap;

        // Write the spawn area
        out << Key << "area" << Value << BeginSeq << Block;
        out << Flow << BeginMap;
        out << Key << "x" << spawn.bottomLeft.x;
        out << Key << "y" << spawn.bottomLeft.y;
        out << Key << "z" << spawn.bottomLeft.z;
        out << EndMap << Block;
        out << Flow << BeginMap;
        out << Key << "x" << spawn.topRight.x;
        out << Key << "y" << spawn.topRight.y;
        out << Key << "z" << spawn.topRight.z;
        out << EndMap << Block;
        out << EndSeq;

        // Write the mob spawns
        out << Key << "spawns" << Value << BeginSeq << Block;
        for (auto&& mob: spawn.spawns)
        {
            out << Flow << BeginMap;
            out << Key << "id" << mob.id;
            out << Key << "quantity" << mob.count;
            out << EndMap << Block;
        }
        out << EndSeq;

        // End the spawn
        out << EndMap;
        out << EndMap;
    }
}