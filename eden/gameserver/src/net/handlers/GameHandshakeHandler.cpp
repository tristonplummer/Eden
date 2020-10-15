#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/GameHandshake.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/CharacterScreenService.hpp>
#include <shaiya/game/service/ServiceContext.hpp>
#include <shaiya/game/service/WorldApiService.hpp>

#include <crypto++/osrng.h>

using namespace shaiya::net;

/**
 * Handles an incoming game handshake request.
 * @param session   The session instance.
 * @param request   The inbound handshake request.
 */
void handleHandshake(Session& session, const GameHandshakeRequest& request)
{
    auto& game     = dynamic_cast<GameSession&>(session);  // The game session
    auto& api      = game.context().getApiService();
    auto& identity = request.identity;

    // Attempt to find the transfer request for a provided identity.
    auto transfer = api.getTransferForIdentity(identity);
    if (!transfer)
    {
        LOG(INFO) << "Couldn't find transfer request for user id: " << request.userId;
        game.close();
        return;
    }

    // If the transfer doesn't originate from the same ip address, disconnect the session.
    if (game.remoteAddress() != transfer->ipaddress())
    {
        LOG(INFO) << "Transfer request was found for user id: " << request.userId << ", but request address "
                  << game.remoteAddress() << " did not match transfer origin address " << transfer->ipaddress();
        game.close();
        return;
    }

    // The aes key iv, and xor key
    std::array<byte, 16> key{ 0 };
    std::array<byte, 16> iv{ 0 };
    std::array<byte, 16> xorKey{ 0 };

    // Populate the AES values
    std::memcpy(key.data(), transfer->key().data(), key.size());
    std::memcpy(iv.data(), transfer->iv().data(), iv.size());

    // Generate the XOR key
    CryptoPP::AutoSeededRandomPool prng;
    prng.GenerateBlock((byte*)xorKey.data(), xorKey.size());

    // Initialise the encryption based on the previous AES keys.
    game.initEncryption(key, iv, xorKey);

    // Generate the expanded key to use for game world encryption and provide it to the client
    GameHandshakeResponse response;
    std::memcpy(response.expandedKeySeed.data(), xorKey.data(), xorKey.size());
    game.write(response);

    // Set the session's user id
    game.setUserId(transfer->userid());

    // Show the character selection screen
    auto& charScreen = game.context().getCharScreen();
    charScreen.display(game);
}

/**
 * A template specialization used for registering a game handshake request handler.
 */
template<>
void PacketRegistry::registerPacketHandler<GameHandshakeOpcode>()
{
    registerHandler<GameHandshakeOpcode, GameHandshakeRequest>(&handleHandshake, ExecutionType::Asynchronous);
}