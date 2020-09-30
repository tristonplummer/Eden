#pragma once

namespace shaiya::net
{
    /**
     * Represents how a packet handler should be executed.
     */
    enum class ExecutionType
    {
        Synchronous,
        Asynchronous
    };
}