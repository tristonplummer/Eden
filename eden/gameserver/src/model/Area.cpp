#include <shaiya/common/util/Prng.hpp>
#include <shaiya/game/model/Area.hpp>

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
    auto& prng = shaiya::Prng::the();

    auto x = prng.random(bottomLeft_.x(), topRight_.x());
    auto z = prng.random(bottomLeft_.z(), topRight_.z());

    // Return a random point in this area
    return Position(bottomLeft_.map(), x, bottomLeft_.y(), z);
}

/**
 * Gets a random point with this area as the center.
 * @param range The range around the area.
 * @return      The random point.
 */
Position Area::randomPoint(float range) const
{
    auto& prng = shaiya::Prng::the();

    // The range of coordinates
    auto x = prng.random(bottomLeft_.x(), topRight_.x(), range);
    auto z = prng.random(bottomLeft_.z(), topRight_.z(), range);

    // Return a random point in this area
    return Position(bottomLeft_.map(), x, bottomLeft_.y(), z);
}