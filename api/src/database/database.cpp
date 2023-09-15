#include "database/database.hpp"

pqxx::connection database::connect()
{
    pqxx::connection connection(CONNECTION_STRING);
    return connection;
}

pqxx::result database::execute(pqxx::connection &connection, const std::string &sql)
{
    pqxx::work work(connection);
    pqxx::result result = work.exec(sql);
    work.commit();
    return result;
}