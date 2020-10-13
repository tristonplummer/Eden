#pragma once
#include <shaiya/game/world/model/Entity.hpp>

#include <vector>

namespace shaiya::game
{
    /**
     * An entity container is a container that manages the storage and lifetime of Entities. Entities that are inserted into
     * this container have their id set to the index in the local array.
     * @tparam T
     */
    template<typename T>
    class EntityContainer
    {
    public:
        /**
         * Initialises this entity container with a specified maximum capacity.
         * @param capacity  The maximum capacity of elements.
         */
        explicit EntityContainer(size_t capacity = 2048)
        {
            elements_.resize(capacity);
        }

        /**
         * Adds an entity element to this container.
         * @param element   The entity.
         * @return          If the entity was successfully added.
         */
        bool add(std::shared_ptr<T> element)
        {
            for (auto i = 0; i < elements_.size(); i++)
            {
                if (elements_[i] == nullptr)
                {
                    element->setId(i);
                    elements_[i] = std::move(element);
                    return true;
                }
            }
            return false;
        }

        /**
         * Removes an entity from this container.
         * @param element   The entity.
         */
        void remove(const std::shared_ptr<T>& element)
        {
            elements_[element->id()] = nullptr;
        }

    private:
        /**
         * The vector of elements that are being stored here.
         */
        std::vector<std::shared_ptr<T>> elements_;
    };
}