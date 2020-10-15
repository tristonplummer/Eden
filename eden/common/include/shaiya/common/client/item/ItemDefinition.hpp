#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::client
{
    /**
     * Represents the definition of an item.
     */
    struct ItemDefinition
    {
        /**
         * The name of the item.
         */
        VariableString<64> name;

        /**
         * The description of the item.
         */
        VariableString<256> description;

        /**
         * The id of the item. This is calculated at (type * 1000) + typeId;
         */
        uint32_t id{};

        /**
         * The type of the item.
         */
        uint8_t type{};

        /**
         * The type id of the item.
         */
        uint8_t typeId{};

        /**
         * The model id of the item.
         */
        uint8_t model{};

        /**
         * The icon that the item uses.
         */
        uint8_t icon{};

        /**
         * The level required to use the item.
         */
        uint16_t level{};

        /**
         * The required faction.
         */
        uint8_t faction{};

        /**
         * If this item is usable by Fighter classes.
         */
        bool usableByFighter{};

        /**
         * If this item is usable by Defender classes.
         */
        bool usableByDefender{};

        /**
         * If this item is usable by Ranger classes.
         */
        bool usableByRanger{};

        /**
         * If this item is usable by Archer classes.
         */
        bool usableByArcher{};

        /**
         * If this item is usable by Mage classes.
         */
        bool usableByMage{};

        /**
         * If this item is usable by Priest classes.
         */
        bool usableByPriest{};

        /**
         * The game mode required to use this item.
         */
        uint8_t requiredMode{};

        /**
         * TODO: Refactor this
         */
        uint8_t unknown1{};

        /**
         * TODO: Refactor this
         */
        uint8_t unknown2{};

        /**
         * TODO: Refactor this
         */
        uint16_t reqStr{};

        /**
         * The colour that the item name gets shown as.
         */
        uint16_t nameColour{};

        /**
         * TODO: Refactor this
         */
        uint16_t reqRec{};

        /**
         * TODO: Refactor this
         */
        uint16_t reqInt{};

        /**
         * The maximum reroll value.
         */
        uint16_t maxReroll{};

        /**
         * The enchant material required to enchant this item.
         */
        uint32_t enchantMaterial{};

        /**
         * TODO: Refactor this
         */
        uint16_t reqVg{};

        /**
         * TODO: Refactor this
         */
        uint8_t reqOg{};

        /**
         * TODO: Refactor this
         */
        uint8_t reqIg{};

        /**
         * The attack range of the item.
         */
        uint16_t attackRange{};

        /**
         * The attack speed of the item.
         */
        uint8_t attackSpeed{};

        /**
         * The element of the item.
         */
        uint8_t element{};

        /**
         * The item's special effect (probably won't be used on this server)
         */
        uint8_t special{};

        /**
         * The number of lapis slots the item has available for linking.
         */
        uint8_t lapisSlots{};

        /**
         * The maximum endurance of the item.
         */
        uint16_t endurance{};

        /**
         * The minimum attack of the item.
         */
        uint16_t minAttack{};

        /**
         * The attack bonus (difference between max and min attack)
         */
        uint16_t attackBonus{};

        /**
         * TODO: Refactor this
         */
        uint16_t effect3{};

        /**
         * TODO: Refactor this
         */
        uint16_t effect4{};

        /**
         * The amount of hitpoints this item gives the player.
         */
        uint16_t hitpoints{};

        /**
         * The amount of stamina this item gives the player.
         */
        uint16_t stamina{};

        /**
         * The amount of mana this item gives the player.
         */
        uint16_t mana{};

        /**
         * The amount of strength this item gives the player.
         */
        uint16_t strength{};

        /**
         * The amount of dexterity this item gives the player.
         */
        uint16_t dexterity{};

        /**
         * The amount of reaction this item gives the player.
         */
        uint16_t reaction{};

        /**
         * The amount of intelligence this item gives the player.
         */
        uint16_t intelligence{};

        /**
         * The amount of wisdom this item gives the player.
         */
        uint16_t wisdom{};

        /**
         * The amount of luck this item gives the player.
         */
        uint16_t luck{};

        /**
         * The movement speed bonus provided by this item.
         */
        uint8_t movementSpeed{};

        /**
         * TODO: Refactor this
         */
        uint8_t exp{};

        /**
         * The value required to purchase this item from a merchant
         */
        uint32_t purchaseValue{};

        /**
         * The gold value given when sold to a merchant.
         */
        uint32_t sellValue{};

        /**
         * The drop grade.
         */
        uint16_t grade{};

        /**
         * TODO: Refactor this
         */
        uint16_t drop{};

        /**
         * TODO: Refactor this
         */
        uint8_t server{};

        /**
         * The maximum stack size.
         */
        uint8_t maxStack{};
    } PACKED;
}