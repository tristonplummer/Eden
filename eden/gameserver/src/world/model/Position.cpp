#include <shaiya/game/world/model/Position.hpp>

using namespace shaiya::game;

/**
 * Creates a position from a set of coordinates.
 * @param map   The map id.
 * @param x     The x coordinate.
 * @param y     The y coordinate.
 * @param z     The z coordinate.
 */
Position::Position(uint16_t map, float x, float y, float z): map_(map), x_(x), y_(y), z_(z)
{
}

/**
 * Checks if a position is equal to this one.
 * @param other The other position.
 * @return      If the two positions are equal.
 */
bool Position::operator==(const Position& other) const
{
    return map_ == other.map_ && x_ == other.x_ && y_ == other.y_ && z_ == other.z_;
}
