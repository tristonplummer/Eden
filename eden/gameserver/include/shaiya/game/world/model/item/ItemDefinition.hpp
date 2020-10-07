#pragma once
#include <string>

namespace shaiya::game
{
    /**
     * Represents a static definition about an item id. This definition can include things such as it's maximum durability,
     * if the item can be equipped (and if so, in what slot), the maximum stack count, the gold value, etc...
     */
    struct ItemDefinition
    {
        /**
         * The id of this definition.
         */
        size_t id{ 0 };

        /**
         * The amount of strength this item adds.
         */
        size_t strength{ 0 };

        /**
         * The amount of dexterity this item adds.
         */
        size_t dexterity{ 0 };

        /**
         * The amount of reaction this item adds.
         */
        size_t reaction{ 0 };

        /**
         * The amount of intelligence this item adds.
         */
        size_t intelligence{ 0 };

        /**
         * The amount of wisdom this item adds.
         */
        size_t wisdom{ 0 };

        /**
         * The amount of luck this item adds.
         */
        size_t luck{ 0 };

        /**
         * The name of the item.
         */
        std::string name{};
    };
}