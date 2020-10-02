#include <shaiya/login/net/LoginSession.hpp>

#include <regex>

using namespace shaiya::login;
using namespace shaiya::net;

/**
 * The name of the login authentication statement
 */
constexpr auto LOGIN_AUTH_STATEMENT = "login_auth";

/**
 * Initialises this service, and prepares the queries to use in the database.
 * @param db    The database service.
 */
AuthenticationService::AuthenticationService(shaiya::database::DatabaseService& db): db_(db)
{
    db.prepare(LOGIN_AUTH_STATEMENT, "");
}

/**
 * Processes a login request.
 * @param session   The session that is sending the login request.
 * @param username  The provided username.
 * @param password  The provided password.
 */
void AuthenticationService::login(LoginSession& session, const std::string& username, const std::string& password)
{
    // Send an error response
    auto sendError = [&](LoginStatus status) {
        LoginResponse response;
        response.status = status;
        session.write(response, 3);
    };

    // An invalid login response
    LoginResponse response;
    response.status   = LoginStatus::InvalidCredentials;
    response.identity = session.identity();
    session.write(response);
}