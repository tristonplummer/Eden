#pragma once
#include <shaiya/game/Forward.hpp>

namespace shaiya::game
{
    /**
     * Represents the appearance of a character.
     */
    class Appearance
    {
    public:
        /**
         * Initialise the appearance for a character.
         * @param character The character instance.
         */
        explicit Appearance(Character& character);

        /**
         * Sets the hair for this appearance.
         * @param hair  The hair value
         */
        void setHair(uint8_t hair);

        /**
         * Sets the face for this appearance.
         * @param face  The face value.
         */
        void setFace(uint8_t face);

        /**
         * Sets the height for this appearance.
         * @param height    The height value.
         */
        void setHeight(uint8_t height);

        /**
         * Sets the gender for this appearance.
         * @param gender    The gender value.
         */
        void setGender(ShaiyaGender gender);

        /**
         * Gets the hair of this appearance.
         * @return  The hair.
         */
        [[nodiscard]] uint8_t hair() const
        {
            return hair_;
        }

        /**
         * Gets the face of this appearance.
         * @return  The face.
         */
        [[nodiscard]] uint8_t face() const
        {
            return face_;
        }

        /**
         * Gets the height of this appearance.
         * @return  The height.
         */
        [[nodiscard]] uint8_t height() const
        {
            return height_;
        }

        /**
         * Gets the gender of this appearance.
         * @return  The gender.
         */
        [[nodiscard]] ShaiyaGender gender() const
        {
            return gender_;
        }

    private:
        /**
         * The character that owns this appearance.
         */
        Character& character_;

        /**
         * The hair of the character.
         */
        uint8_t hair_{ 0 };

        /**
         * The face of the character.
         */
        uint8_t face_{ 0 };

        /**
         * The height of the character.
         */
        uint8_t height_{ 0 };

        /**
         * The gender of the character.
         */
        ShaiyaGender gender_{ ShaiyaGender::Male };
    };
}