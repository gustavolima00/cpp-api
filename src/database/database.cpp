#include "database/database.hpp"

pqxx::connection database::connect()
{
    const char *connection_string = std::getenv("DATABASE_CONNECTION_STRING");
    if (connection_string == nullptr)
    {
        connection_string = DEFAULT_CONNECTION_STRING;
    }
    pqxx::connection connection(connection_string);
    return connection;
}