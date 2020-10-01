#include <shaiya/login/net/LoginSession.hpp>

#include <chrono>
#include <thread>

using namespace shaiya::login;

/**
 * The maximum number of worlds to display on the world list.
 */
constexpr size_t WorldListCapacity = 5;

/**
 * The interval between world server pings.
 */
constexpr auto PingInterval = 10;

/**
 * Initialises this world service.
 */
WorldService::WorldService()
{
    // Start a thread to periodically update the world servers' data.
    thread_ = std::thread([this] {
        while (running_)
        {
            for (auto&& world: worlds_)
                world.update();
            std::this_thread::sleep_for(std::chrono::seconds(PingInterval));
        }
    });
    thread_.detach();
}

/**
 * Gets a world with a specified id. Returns a null pointer in the event that no world
 * with the specified id exists.
 * @param id    The world id.
 * @return      The world instance.
 */
WorldServer* WorldService::getWorld(uint8_t id)
{
    for (auto&& world: worlds_)
    {
        if (world.getId() == id)
            return &world;
    }
    return nullptr;
}

/**
 * Sends the world list to a session.
 * @param session   The session.
 */
void WorldService::sendWorldList(shaiya::net::LoginSession& session)
{
    using namespace shaiya::net;
    WorldListPacket<WorldListCapacity> worldList;  // The world list packet
    worldList.count = worlds_.size();

    // Loop over the worlds
    for (auto i = 0; i < std::min(WorldListCapacity, worlds_.size()); i++)
    {
        auto& world = worlds_.at(i);
        auto& entry = worldList.servers.at(i);

        entry.id             = world.getId();
        entry.name           = world.getName();
        entry.playerCount    = world.playerCount();
        entry.playerCapacity = world.playerCapacity();

        if (world.isOnline())
            entry.status = WorldStatus::Normal;
    }

    // Send the world list to the session
    session.write(worldList, 3 + (worldList.count * sizeof(WorldListEntry)));
}