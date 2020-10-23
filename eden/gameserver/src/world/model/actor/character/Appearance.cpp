#include <shaiya/game/world/model/actor/player/Appearance.hpp>
#include <shaiya/game/world/model/actor/player/Player.hpp>

using namespace shaiya;
using namespace shaiya::game;

/**
 * Initialise the appearance for a character.
 * @param player The character instance.
 */
Appearance::Appearance(Player& player): player_(player)
{
}

/**
 * Sets the hair for this appearance.
 * @param hair  The hair value
 */
void Appearance::setHair(uint8_t hair)
{
    if (hair_ == hair)
        return;
    hair_ = hair;
    player_.flagUpdate(UpdateFlag::Appearance);
}

/**
 * Sets the face for this appearance.
 * @param face  The face value.
 */
void Appearance::setFace(uint8_t face)
{
    if (face_ == face)
        return;
    face_ = face;
    player_.flagUpdate(UpdateFlag::Appearance);
}

/**
 * Sets the height for this appearance.
 * @param height    The height value.
 */
void Appearance::setHeight(uint8_t height)
{
    if (height_ == height)
        return;
    height_ = height;
    player_.flagUpdate(UpdateFlag::Appearance);
}

/**
 * Sets the gender for this appearance.
 * @param gender    The gender value.
 */
void Appearance::setGender(ShaiyaGender gender)
{
    if (gender_ == gender)
        return;
    gender_ = gender;
    player_.flagUpdate(UpdateFlag::Appearance);
}