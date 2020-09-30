#pragma once
#include <shaiya/common/net/Session.hpp>
#include <shaiya/common/net/packet/ExecutionType.hpp>
#include <shaiya/common/net/packet/Packet.hpp>
#include <shaiya/common/util/Async.hpp>

namespace shaiya::net
{
    /**
     * A utility class that handles the registration of execution of packet handlers.
     */
    class PacketRegistry
    {
    public:
        /**
         * Initialises the opcodes to handle.
         */
        PacketRegistry();

        /**
         * Attempts to handle an inbound packet.
         * @param session   The session instance.
         * @param opcode    The opcode of the packet.
         * @param length    The length of the packet.
         * @param payload   The raw packet data.
         */
        void execute(Session& session, uint16_t opcode, uint16_t length, const char* payload);

        /**
         * Logs an unhandled packet.
         * @param session   The session that sent the packet.
         * @param opcode    The opcode of the packet.
         * @param length    The length of the packet.
         * @param payload   The payload of the packet.
         */
        static void logUnhandled(Session& session, size_t opcode, size_t length, const char* payload);

        /**
         * Registers a packet handler. This also attempts to instantiate the next opcodes
         * packet handler. This is so we can ensure all handlers are instantiated without needing
         * to explicitly add more code to this class every time.
         * @tparam Opcode   The opcode of the packet.
         * @tparam T        The packet type to convert the data to.
         * @param func      The handler function to execute.
         * @param predicate A predicate that is checked to see if the handler function should even bother executing
         */
        template<size_t Opcode, typename T>
        void registerHandler(const std::function<void(Session&, const T&)>& func,
                             ExecutionType execution = ExecutionType::Synchronous)
        {
            handlers_[Opcode] = [this, func, execution](Session& session, size_t length, const char* payload) {
                // The copying of this packet is done early, as the Session base class empties
                // it's buffer when the handler function returns, thus in the event of an asynchronous
                // handler being executed, the buffer would likely be empty.
                const auto packet = toPacket<T>(payload, length);

                // A simple helper function that moves the packet, executes the handler function
                // and then deletes the packet. This is just so that we don't need to repeat ourselves
                // depending on the execution type.
                auto process = [&, func, packet = std::move(packet)]() { func(session, packet); };

                // Execute the handler depending on the execution type.
                if (execution == ExecutionType::Synchronous)
                    process();
                else if (execution == ExecutionType::Asynchronous)
                    ASYNC(process)
            };
        }

        /**
         * Gets the registry singleton.
         * @return  The packet registry.
         */
        static PacketRegistry& the()
        {
            static PacketRegistry instance;
            return instance;
        }

    private:
        /**
         * Attempts to register a packet handler for an opcode.
         * @tparam Opcode   The packet opcode.
         */
        template<size_t Opcode>
        void registerPacketHandler()
        {
        }

        /**
         * A map of packet opcodes to their handler functions.
         */
        std::map<size_t, std::function<void(Session&, size_t, const char*)>> handlers_;
    };
}