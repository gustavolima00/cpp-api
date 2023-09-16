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

void people_repository::create_pearson(const Pearson &pearson)
{
  auto connection = database::connect();
  pqxx::work txn(connection);

  pqxx::result result = txn.exec_params(
      "INSERT INTO people (nome, apelido, nascimento, stack) VALUES ($1, $2, $3, $4) RETURNING id",
      pearson.nome,
      pearson.apelido,
      pearson.nascimento,
      pearson.stack);

  txn.commit();
}

Pearson people_repository::get_pearson(string &id)
{
  auto connection = database::connect();
  pqxx::work txn(connection);

  pqxx::result result = txn.exec_params(
      "SELECT nome, apelido, nascimento, stack FROM people WHERE id = $1",
      id);

  if (result.empty())
  {
    throw std::runtime_error("Pearson não encontrada");
  }
  auto row = result[0];

  string nome = row[0].as<string>();
  string apelido = row[1].as<string>();
  string nascimento = row[2].as<string>();
  string raw_stack = row[3].as<string>();
  vector<string> stack = parse_pg_array(raw_stack);

  cout << "id: " << id << endl;
  cout << "nome: " << nome << endl;
  cout << "apelido: " << apelido << endl;
  cout << "nascimento: " << nascimento << endl;
  cout << "raw_stack: " << raw_stack << endl;

  return Pearson(id, nome, apelido, nascimento, stack);
}

vector<Pearson> people_repository::search_people(string &term)
{
  auto connection = database::connect();
  pqxx::work txn(connection);

  pqxx::result result = txn.exec_params(
      "SELECT id, nome, apelido, nascimento, stack FROM people WHERE nome ILIKE $1 OR apelido ILIKE $1",
      "%" + term + "%");

  vector<Pearson> people;
  for (auto row : result)
  {
    string id = row[0].as<string>();
    string nome = row[1].as<string>();
    string apelido = row[2].as<string>();
    string nascimento = row[3].as<string>();
    string raw_stack = row[4].as<string>();
    vector<string> stack = parse_pg_array(raw_stack);

    people.push_back(Pearson(id, nome, apelido, nascimento, stack));
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