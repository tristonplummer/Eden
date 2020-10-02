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
    db.prepare(LOGIN_AUTH_STATEMENT, "SELECT userid, status, privilege FROM userdata.login($1, $2, $3);");
}

/**
 * Processes a login request.
 * @param session   The session that is sending the login request.
 * @param username  The provided username.
 * @param password  The provided password.
 */
void AuthenticationService::login(LoginSession& session, const std::string& username, const std::string& password)
{
    // A helper function used to send an error to a session.
    auto sendError = [&](LoginStatus status) {
        LoginResponse response;
        response.status = status;
        session.write(response, 3);
    };

    // Attempt to submit a login request to the database
    try
    {
        auto connection = db_.connection();
        pqxx::work tx(*connection);

        // Execute the login authentication function
        auto rows = tx.exec_prepared(LOGIN_AUTH_STATEMENT, username, password, session.remoteAddress());
        if (rows.empty())
            return sendError(LoginStatus::InvalidCredentials);

        // Get the data returned from the login function
        auto row       = rows.front();
        auto userId    = row["userid"].as<uint32_t>();
        auto status    = static_cast<LoginStatus>(row["status"].as<uint32_t>());
        auto privilege = row["privilege"].as<uint32_t>();

        // If the status wasn't successful, send it as an error
        if (status != LoginStatus::Success)
            return sendError(status);

        // Send the world list
        auto& worldService = session.context().getWorldService();
        worldService.sendWorldList(session);

        // Set the session's user id
        session.setUserId(userId);

        // Send the successful login response
        LoginResponse response;
        response.status    = status;
        response.userId    = userId;
        response.privilege = privilege;
        response.identity  = session.identity();
        session.write(response);
    }
    catch (const std::exception& e)
    {
        sendError(LoginStatus::CannotConnect);  // Cannot connect to the login server
    }
}