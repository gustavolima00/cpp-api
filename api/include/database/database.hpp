#pragma once
#include <pqxx/pqxx>

// #define CONNECTION_STRING "postgresql://postgres:postgres@database:5432/postgres"
#define CONNECTION_STRING "postgresql://postgres:postgres@localhost:5432/postgres"

namespace database
{
    void connect();
}