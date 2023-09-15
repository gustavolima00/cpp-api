#include "database/database.hpp"

void database::connect()
{
    pqxx::connection c(CONNECTION_STRING);
}
