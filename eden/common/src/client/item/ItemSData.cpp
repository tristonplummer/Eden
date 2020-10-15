#include <shaiya/common/client/item/ItemSData.hpp>

#include <fstream>

using namespace shaiya::client;

/**
 * Initialises this ItemSData by reading item entries from a file path
 * @param path  The path to the item sdata.
 */
ItemSData::ItemSData(const std::string& path)
{
    // Open a stream to the item sdata file
    std::ifstream stream(path, std::ios::in | std::ios::binary);

    // The maximum item type
    uint32_t maxType;
    uint32_t maxTypeId;
    stream.read(reinterpret_cast<char*>(&maxType), sizeof(maxType));

    // Loop over the item types
    for (auto type = 1; type <= maxType; type++)
    {
        stream.read(reinterpret_cast<char*>(&maxTypeId), sizeof(maxTypeId));  // Read the max type id for this type.
        for (auto typeId = 1; typeId <= maxTypeId; typeId++)
        {
            decode(stream, type, typeId);
        }
    }
}

/**
 * Decodes the stream for an item entry.
 * @param stream    The file stream.
 * @param type      The item type.
 * @param typeId    The item type id.
 */
void ItemSData::decode(std::ifstream& stream, size_t type, size_t typeId)
{
    // A helper function to read a string that is prefixed by a 32-bit length
    auto readString = [&]() -> std::string {
        uint32_t length;
        stream.read(reinterpret_cast<char*>(&length), sizeof(length));

        std::string text;
        text.resize(length);
        stream.read(text.data(), length);
        return text;
    };

    // The item definition
    auto def    = new ItemDefinition{};
    auto itemId = (type * 1000) + typeId;
    def->id     = itemId;

    // Read the name and description of the item
    def->name        = readString();
    def->description = readString();

    // The size of the remaining structure
    stream.read(reinterpret_cast<char*>(&def->type), sizeof(ItemDefinition) - offsetof(ItemDefinition, type));
    stream.ignore(50);
    definitions_[itemId] = def;
}

/**
 * Gets the item definition for a given id.
 * @param id    The item id.
 * @return      The item definition.
 */
const ItemDefinition* ItemSData::forId(uint32_t id) const
{
    if (!definitions_.contains(id))
        return nullptr;
    return definitions_.at(id);
}

/**
 * Gets the item definition for a given id.
 * @param type      The item type.
 * @param typeId    The item type id.
 * @return          The item definition.
 */
const ItemDefinition* ItemSData::forId(uint8_t type, uint8_t typeId) const
{
    return forId((type * 1000) + typeId);
}