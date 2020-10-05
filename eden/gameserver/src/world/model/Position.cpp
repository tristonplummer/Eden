#include <shaiya/game/world/model/Position.hpp>

using namespace shaiya::game;

/**
 * The viewport distance of a player.
 */
constexpr auto VIEWPORT_DISTANCE = 60;

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
 * Checks if a position is within distance of another.
 * @param other     The other position.
 * @param distance  The distance.
 * @return          If the other position is in distance of this position.
 */
bool Position::isWithinDistance(const Position& other, float distance) const
{
    auto deltaX = x_ - other.x_;
    auto deltaY = y_ - other.y_;
    auto deltaZ = z_ - other.z_;
    return map_ == other.map_ && deltaX <= distance && deltaY <= distance && deltaZ <= distance;
}

/**
 * Checks if a position is within viewport distance of another.
 * @param other     The other position.
 * @return          If the other position is in viewport distance of this position.
 */
bool Position::isWithinViewportDistance(const Position& other) const
{
    return isWithinDistance(other, VIEWPORT_DISTANCE);
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
