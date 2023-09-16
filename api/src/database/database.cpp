#include "database/database.hpp"

pqxx::connection database::connect()
{
    pqxx::connection connection(CONNECTION_STRING);
    return connection;
}