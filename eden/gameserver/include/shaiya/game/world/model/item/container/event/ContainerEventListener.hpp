#pragma once

namespace shaiya::game
{
    class ItemContainer;

    /**
     * A utility class that listens for events performed by a container.
     */
    class ContainerEventListener
    {
    public:

        /**
         * Synchronises the container with this listener.
         * @param container The container that was updated.
         */
        virtual void sync(const ItemContainer& container) = 0;
    };
}