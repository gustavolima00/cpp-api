#include "repositories/people_repository.hpp"

std::vector<std::string> parse_pg_array(const std::string &pg_array)
{
  std::vector<std::string> result;
  std::string element;
  for (std::size_t i = 1; i < pg_array.size() - 1; ++i)
  { // Ignorar as chaves {}
    if (pg_array[i] == ',' || pg_array[i] == '}')
    {
      result.push_back(element);
      element.clear();
    }
    else
    {
      element += pg_array[i];
    }
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

  vector<string> stack{};
  if (pearson.stack.has_value())
  {
    stack = *pearson.stack;
  }

  string stack_as_string;
  for (auto &item : stack)
  {
    stack_as_string += item + ",";
  }

  pqxx::result result = txn.exec_params(
      "INSERT INTO people (name, nickname, birth_date, stack, stack_as_string) VALUES ($1, $2, $3, $4, $5) RETURNING id",
      pearson.name,
      pearson.nickname,
      pearson.birth_date,
      stack,
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
      "SELECT name, nickname, birth_date, stack FROM people WHERE id = $1 LIMIT 1",
      id);

  if (result.empty())
  {
    throw PearsonNotFound();
  }
  auto row = result[0];

  string name = row[0].as<string>();
  string nickname = row[1].as<string>();
  string birth_date = row[2].as<string>();
  string raw_stack = row[3].as<string>();
  vector<string> stack = parse_pg_array(raw_stack);

  cout << "id: " << id << endl;
  cout << "nome: " << name << endl;
  cout << "apelido: " << nickname << endl;
  cout << "nascimento: " << birth_date << endl;
  cout << "raw_stack: " << raw_stack << endl;

  return Pearson(id, name, nickname, birth_date, stack);
}

vector<Pearson> people_repository::search_people(string &term)
{
  auto connection = database::connect();
  pqxx::work txn(connection);

  pqxx::result result = txn.exec_params(
      "SELECT id, name, nickname, birth_date, stack FROM people WHERE name % $1 OR nickname % $1 OR stack_as_string % $1 LIMIT 50", term);

  vector<Pearson> people;
  for (auto row : result)
  {
    string id = row[0].as<string>();
    string name = row[1].as<string>();
    string nickname = row[2].as<string>();
    string birth_date = row[3].as<string>();
    string raw_stack = row[4].as<string>();
    vector<string> stack = parse_pg_array(raw_stack);

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