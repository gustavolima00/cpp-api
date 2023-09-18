#include "repositories/people_repository.hpp"

vector<string> stack_string_to_vector(const string &stack_string)
{
  vector<string> result;
  string element;
  for (auto &c : stack_string)
  {
    if (c == ',')
    {
      result.push_back(element);
      element = "";
    }
    else
    {
      element += c;
    }
  }
  if (element != "")
  {
    result.push_back(element);
  }
  return result;
}

string stack_to_string(const vector<string> &stack)
{
  if (stack.empty())
  {
    return "";
  }

  string result = stack[0];
  for (size_t i = 1; i < stack.size(); ++i)
  {
    result += "," + stack[i];
  }
  return result;
}

people_repository::PearsonNotFound::PearsonNotFound() {}

const char *people_repository::PearsonNotFound::what()
{
  return "Pearson não encontrada";
}

string people_repository::create_pearson(const Pearson &pearson)
{
  auto connection = database::connect();
  pqxx::work txn(connection);

  string stack_as_string;
  if (pearson.stack.has_value())
  {
    stack_as_string = stack_to_string(*pearson.stack);
  }

  pqxx::result result = txn.exec_params(
      "INSERT INTO people (name, nickname, birth_date, stack_as_string) VALUES ($1, $2, $3, $4) RETURNING id",
      pearson.name,
      pearson.nickname,
      pearson.birth_date,
      stack_as_string);
  txn.commit();

  auto row = result[0];
  string id = row[0].as<string>();
  return id;
}

Pearson people_repository::get_pearson(string &id)
{
  auto connection = database::connect();
  pqxx::work txn(connection);

  pqxx::result result = txn.exec_params(
      "SELECT name, nickname, birth_date, stack_as_string FROM people WHERE id = $1 LIMIT 1",
      id);

  if (result.empty())
  {
    throw PearsonNotFound();
  }
  auto row = result[0];

  string name = row[0].as<string>();
  string nickname = row[1].as<string>();
  string birth_date = row[2].as<string>();
  string stack_as_string = row[3].as<string>();
  vector<string> stack = stack_string_to_vector(stack_as_string);

  return Pearson(id, name, nickname, birth_date, stack);
}

vector<Pearson> people_repository::search_people(string &term)
{
  auto connection = database::connect();
  pqxx::work txn(connection);

  pqxx::result result = txn.exec_params(
      "SELECT id, name, nickname, birth_date, stack_as_string FROM people WHERE name LIKE $1 OR nickname LIKE $1 OR stack_as_string LIKE $1",
      "%" + term + "%");

  vector<Pearson> people;
  for (auto row : result)
  {
    string id = row[0].as<string>();
    string name = row[1].as<string>();
    string nickname = row[2].as<string>();
    string birth_date = row[3].as<string>();
    string stack_as_string = row[4].as<string>();
    vector<string> stack = stack_string_to_vector(stack_as_string);

    people.push_back(Pearson(id, name, nickname, birth_date, stack));
  }

  return people;
}

int people_repository::count_people()
{
  auto connection = database::connect();
  pqxx::work txn(connection);

  pqxx::result result = txn.exec("SELECT COUNT(*) FROM people");
  auto row = result[0];
  int count = row[0].as<int>();
  return count;
}
