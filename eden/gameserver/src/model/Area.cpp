#include <shaiya/game/model/Area.hpp>

#include <random>

using namespace shaiya::game;

/**
 * Constructs the area.
 * @param bottomLeft    The bottom left position.
 * @param topRight      The top right position.
 */
Area::Area(Position bottomLeft, Position topRight): bottomLeft_(bottomLeft), topRight_(topRight), prng_(rd_)
{
}

/**
 * Gets a random position in this area.
 * @return  The random point.
 */
Position Area::randomPoint() const
{
    // The range of coordinates
    std::uniform_real_distribution<> xDist(bottomLeft_.x(), topRight_.x());
    std::uniform_real_distribution<> zDist(bottomLeft_.z(), topRight_.z());

    // Return a random point in this area
    return Position(bottomLeft_.map(), xDist(prng_), bottomLeft_.y(), zDist(prng_));
}