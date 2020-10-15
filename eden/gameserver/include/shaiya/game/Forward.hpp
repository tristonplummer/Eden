#pragma once
#include <shaiya/common/DataTypes.hpp>

#include <cstddef>
#include <memory>

namespace shaiya::database
{
    class DatabaseService;
}

namespace shaiya::net
{
    class GameSession;
    class GameTcpServer;
}

namespace shaiya::game
{
    // Entity
    class Entity;
    class Position;

    // Actors
    class Actor;
    class StatSet;
    class Npc;
    class Character;
    class Appearance;
    class ActionBar;

    // Items
    class Item;
    class GroundItem;
    class ItemContainer;
    class InventoryContainer;
    class EquipmentContainer;
    class ContainerEventListener;
    class EquipmentEventListener;
    class InventoryEventListener;

    // Maps
    class Map;
    class MapCell;
    class MapRepository;

    // Commands
    class Command;
    class CommandManager;
    class SpawnItemCommand;
    class SpawnNpcCommand;
    class TeleportCommand;

    // Scheduling
    class Scheduler;
    class ScheduledTask;
    class HealthNormalizationTask;

    // Synchronization
    class ClientSynchronizer;
    class ParallelClientSynchronizer;
    class CharacterSynchronizationTask;
    class MapSynchronizationTask;
    class NpcSynchronizationTask;

    // Serializers
    class CharacterSerializer;
    class DatabaseCharacterSerializer;

    // Services
    class ServiceContext;
    class CharacterScreenService;
    class GameWorldService;
    class ItemDefinitionService;
    class WorldApiService;
}