#include <shaiya/common/db/DatabaseService.hpp>

#include <boost/format.hpp>
#include <glog/logging.h>

using namespace shaiya::database;

/**
 * The number of active connections in the connection pool.
 */
constexpr auto POOL_SIZE = 10;

/**
 * Initialises the database service
 * @param address   The database address
 * @param database  The database name
 * @param username  The name of the user
 * @param password  The password of the user
 */
DatabaseService::DatabaseService(const std::string& address, const std::string& database, const std::string& username,
                                 const std::string& password)
{
    createPool(address, database, username, password);
}

/**
 * Initialises the connection pool
 * @param address   The address of the database server
 * @param database  The name of the database
 * @param username  The user to connect with
 * @param password  The password of the user
 */
void DatabaseService::createPool(const std::string& address, const std::string& database, const std::string& username,
                                 const std::string& password)
{
    std::lock_guard lock{ mutex_ };

    auto fmt = boost::format("host=%1% dbname=%2% user=%3% password=%4%") % address % database % username % password;
    auto connectionString = fmt.str();

    for (size_t i = 0; i < POOL_SIZE; i++)
    {
        auto* con = new pqxx::connection(connectionString);
        pool_.emplace(con);
    }
}

/**
 * Prepares a statement
 * @param name          The name of the statement
 * @param statement     The statement body.
 */
void DatabaseService::prepare(const std::string& name, const std::string& statement)
{
    for (size_t i = 0; i < pool_.size(); i++)
    {
        auto con = connection();
        con->prepare(name, statement);
    }
}

/**
 * Creates a new connection
 * @return  The connection
 */
std::shared_ptr<pqxx::connection> DatabaseService::connection()
{
    std::unique_lock lock{ mutex_ };

    while (pool_.empty())
    {
        condition_.wait(lock);
    }

    auto connection = pool_.front();
    pool_.pop();

    return std::shared_ptr<pqxx::connection>(connection, [this](auto* con) {
        std::unique_lock lock{ mutex_ };
        pool_.push(con);
        condition_.notify_one();
    });
}