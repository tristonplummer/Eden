#include <shaiya/game/model/Area.hpp>

#include <random>

using namespace shaiya::game;

/**
 * Constructs the area.
 * @param bottomLeft    The bottom left position.
 * @param topRight      The top right position.
 */
Area::Area(Position bottomLeft, Position topRight): bottomLeft_(bottomLeft), topRight_(topRight)
{
}

/**
 * Gets a random position in this area.
 * @return  The random point.
 */
Position Area::randomPoint() const
{
    // Initialise a PRNG
    std::random_device rd;
    std::mt19937 prng(rd());

    // The range of coordinates
    std::uniform_real_distribution<> xDist(bottomLeft_.x(), topRight_.x());
    std::uniform_real_distribution<> zDist(bottomLeft_.z(), topRight_.z());

    // Return a random point in this area
    return Position(bottomLeft_.map(), xDist(prng), bottomLeft_.y(), zDist(prng));
}

/**
 * Gets a random point with this area as the center.
 * @param range The range around the area.
 * @return      The random point.
 */
[[nodiscard]] Position Area::randomPoint(float range) const
{
    // Initialise a PRNG
    std::random_device rd;
    std::mt19937 prng(rd());

    // The range of coordinates
    std::uniform_real_distribution<> xDist(bottomLeft_.x() - range, topRight_.x() + range);
    std::uniform_real_distribution<> zDist(bottomLeft_.z() - range, topRight_.z() + range);

    // Return a random point in this area
    return Position(bottomLeft_.map(), xDist(prng), bottomLeft_.y(), zDist(prng));
}