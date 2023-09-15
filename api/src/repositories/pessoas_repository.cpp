#include "repositories/pessoas_repository.hpp"

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

void pessoas_repository::create_pessoa(const Pessoa &pessoa)
{
  auto connection = database::connect();
  pqxx::work txn(connection);

  pqxx::result result = txn.exec_params(
      "INSERT INTO pessoas (nome, apelido, nascimento, stack) VALUES ($1, $2, $3, $4) RETURNING id",
      pessoa.nome,
      pessoa.apelido,
      pessoa.nascimento,
      pessoa.stack);

  txn.commit();
}

Pessoa pessoas_repository::get_pessoa(string &id)
{
  auto connection = database::connect();
  pqxx::work txn(connection);

  pqxx::result result = txn.exec_params(
      "SELECT nome, apelido, nascimento, stack FROM pessoas WHERE id = $1",
      id);

  if (result.empty())
  {
    throw std::runtime_error("Pessoa não encontrada");
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

  return Pessoa(id, nome, apelido, nascimento, stack);
}

vector<Pessoa> pessoas_repository::search_pessoa(string &term)
{
  auto connection = database::connect();
  pqxx::work txn(connection);

  pqxx::result result = txn.exec_params(
      "SELECT id, nome, apelido, nascimento, stack FROM pessoas WHERE nome ILIKE $1 OR apelido ILIKE $1",
      "%" + term + "%");

  vector<Pessoa> pessoas;
  for (auto row : result)
  {
    string id = row[0].as<string>();
    string nome = row[1].as<string>();
    string apelido = row[2].as<string>();
    string nascimento = row[3].as<string>();
    string raw_stack = row[4].as<string>();
    vector<string> stack = parse_pg_array(raw_stack);

    pessoas.push_back(Pessoa(id, nome, apelido, nascimento, stack));
  }

  return pessoas;
}

int pessoas_repository::count_pessoa()
{
  auto connection = database::connect();
  pqxx::work txn(connection);

  pqxx::result result = txn.exec("SELECT COUNT(*) FROM pessoas");
  auto row = result[0];
  int count = row[0].as<int>();
  return count;
}