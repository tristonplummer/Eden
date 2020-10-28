#include <shaiya/common/client/svmap/ServerMap.hpp>

#include <glog/logging.h>

#include <iostream>
#include <yaml-cpp/yaml.h>

/**
 * Converts the npc spawns.
 * @param map   The map.
 */
void convertNpcs(shaiya::client::ServerMap& map);

/**
 * The entry point for the servermap conversion tool.
 * @param argc  The number of command-line arguments.
 * @param argv  The command-line values.
 * @return      The status code.
 */
int main(int argc, char** argv)
{
    using namespace shaiya::client;
    if (argc != 2)
    {
        LOG(ERROR) << "Please provide the path to a SVMap file.";
        return 1;
    }

    ServerMap map(argv[1]);
    convertNpcs(map);
    return 0;
}

/**
 * Converts the npc spawns.
 * @param map   The map.
 */
void convertNpcs(shaiya::client::ServerMap& map)
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