#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode of a login request.
     */
    constexpr auto LoginRequestOpcode = 0xA102;

    /**
     * Represents the result of a login request
     */
    enum class LoginStatus : uint8_t
    {
        Success,
        AccountDoesntExist,
        CannotConnect,
        InvalidCredentials,
        AccountDisabled = 10
    };

    /**
     * Represents a request from the client to the login server, to process a login request
     * for an account with specified credentials.
     */
    struct AccountLoginRequest
    {
        /**
         * The opcode for the login request.
         */
        uint16_t opcode{ LoginRequestOpcode };

        /**
         * The provided username.
         */
        VariableString<32> username;

        /**
         * The provided password.
         */
        VariableString<19> password;
    } PACKED;

    /**
     * Represents the response to a login request.
     */
    struct LoginResponse
    {
        /**
         * The opcode for the login response.
         */
        uint16_t opcode{ LoginRequestOpcode };

        /**
         * The status of a login request.
         */
        LoginStatus status{ LoginStatus::Success };

        /**
         * The user id of the player's account.
         */
        uint32_t userId{ 0 };

        /**
         * The privilege level of the account.
         */
        uint8_t privilege{ 0 };

        /**
         * The identity of the session.
         */
        std::array<char, 16> identity;
    } PACKED;
}