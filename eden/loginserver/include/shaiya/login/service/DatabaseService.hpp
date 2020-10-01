#pragma once

#include <condition_variable>
#include <mutex>
#include <pqxx/pqxx>
#include <queue>
#include <string>

namespace shaiya::login
{
    /**
     * The database service is a utility class which provides access to the PostgreSQL
     * database connection.
     */
    class DatabaseService
    {
    public:
        /**
         * Initialises the database service
         * @param address   The database address
         * @param database  The database name
         * @param username  The name of the user
         * @param password  The password of the user
         */
        DatabaseService(const std::string& address, const std::string& database, const std::string& username,
                        const std::string& password);

        /**
         * Prepares a statement
         * @param name          The name of the statement
         * @param statement     The statement body.
         */
        void prepare(const std::string& name, const std::string& statement);

        /**
         * Creates a new connection
         * @return  The connection
         */
        std::shared_ptr<pqxx::connection> connection();

    public:
        /**
         * Initialises the connection pool
         */
        void createPool(const std::string& address, const std::string& database, const std::string& username,
                        const std::string& password);

        /**
         * The mutex for accessing the connection pool
         */
        std::mutex mutex_;

        /**
         * The variable to wait on for an available connection
         */
        std::condition_variable condition_;

        /**
         * The pool of active connections
         */
        std::queue<pqxx::connection*> pool_;
    };
}