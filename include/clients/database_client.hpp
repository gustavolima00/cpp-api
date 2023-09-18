#pragma once
#include <pqxx/pqxx>
#include <cstdlib>
#include <iostream>
#define DEFAULT_CONNECTION_STRING "postgresql://postgres:postgres@localhost:5432/gustavolima"

namespace database_client
{
    pqxx::connection connect();
}
