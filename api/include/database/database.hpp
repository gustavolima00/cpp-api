#pragma once
#include <pqxx/pqxx>
#include <cstdlib>
#include <iostream>
#define DEFAULT_CONNECTION_STRING "postgresql://postgres:postgres@localhost:5432/postgres"

namespace database
{
    pqxx::connection connect();
}