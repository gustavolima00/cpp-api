#include "clients/database_client.hpp"

pqxx::connection database_client::connect()
{
    const char *connection_string = std::getenv("DATABASE_CONNECTION_STRING");
    if (connection_string == nullptr)
    {
        connection_string = DEFAULT_CONNECTION_STRING;
    }
    pqxx::connection connection(connection_string);
    return connection;
}
