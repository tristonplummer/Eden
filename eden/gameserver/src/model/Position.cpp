#include <shaiya/game/model/Position.hpp>

#include <cmath>

using namespace shaiya::game;

/**
 * The viewport distance of a player.
 */
constexpr auto VIEWPORT_DISTANCE = 90;

/**
 * The interaction distance of a player.
 */
constexpr auto INTERACTION_DISTANCE = 15;

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
 * Sets the x coordinate for this position
 * @param x     The x coordinate
 */
void Position::setX(float x)
{
    x_ = x;
}

/**
 * Sets the y coordinate for this position
 * @param y     The y coordinate
 */
void Position::setY(float y)
{
    y_ = y;
}

/**
 * Sets the z coordinate for this position
 * @param z
 */
void Position::setZ(float z)
{
    z_ = z;
}

/**
 * Translates this position
 * @param x The x offset
 * @param y The y offset
 * @param z The z offset
 * @return  The translated position
 */
Position Position::translate(float x, float y, float z) const
{
    return Position(map_, x_ + x, y_ + y, z_ + z);
}

/**
 * Checks if a position is within distance of another.
 * @param other     The other position.
 * @param distance  The distance.
 * @return          If the other position is in distance of this position.
 */
bool Position::isWithinDistance(const Position& other, float distance) const
{
    auto deltaX = std::abs(x_ - other.x_);
    auto deltaZ = std::abs(z_ - other.z_);
    return map_ == other.map_ && deltaX <= distance && deltaZ <= distance;
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
 * Checks if a position is within interaction distance of another.
 * @param other     The other position.
 * @return          If the other position is in interaction distance of this position.
 */
bool Position::isWithinInteractionDistance(const Position& other) const
{
    return isWithinDistance(other, INTERACTION_DISTANCE);
}

/**
 * Gets the distance between this position and another position.
 * @param other The other position.
 * @return      The distance between the two.
 */
float Position::getDistance(const Position& other) const
{
    auto deltaX = x() - other.x();
    auto deltaZ = z() - other.z();
    return std::sqrt(deltaX * deltaX + deltaZ * deltaZ);
}

/**
 * Checks if these positions are not equal to each other.
 * @param other The other position
 * @return      If the two positions are not equal
 */
bool Position::operator!=(const Position& other) const
{
    return !(*this == other);
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
