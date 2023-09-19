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

string generate_uuid()
{
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, 15);
  uniform_int_distribution<> dis2(8, 11);

  ostringstream ss;
  int i;
  ss << hex;
  for (i = 0; i < 8; i++)
  {
    ss << dis(gen);
  }
  ss << "-";
  for (i = 0; i < 4; i++)
  {
    ss << dis(gen);
  }
  ss << "-4";
  for (i = 0; i < 3; i++)
  {
    ss << dis(gen);
  }
  ss << "-";
  ss << dis2(gen);
  for (i = 0; i < 3; i++)
  {
    ss << dis(gen);
  }
  ss << "-";
  for (i = 0; i < 12; i++)
  {
    ss << dis(gen);
  };
  return ss.str();
}

people_repository::PearsonNotFound::PearsonNotFound() {}

const char *people_repository::PearsonNotFound::what() const noexcept
{
  return "Pearson não encontrada";
}

people_repository::PearsonAlreadyExists::PearsonAlreadyExists() {}

const char *people_repository::PearsonAlreadyExists::what() const noexcept
{
  return "Pearson já existe";
}

bool people_repository::name_exists(string &name)
{
  auto redis_connection = redis_client::connect();
  vector<string> people_as_strings = redis_client::get_all_from_queue(redis_connection, "people");
  for (auto &person_as_string : people_as_strings)
  {
    Pearson person = json_dto::from_json<Pearson>(person_as_string);
    if (person.name == name)
    {
      return true;
    }
  }

  auto connection = database_client::connect();
  pqxx::work txn(connection);

  pqxx::result result = txn.exec_params(
      "SELECT COUNT(*) FROM people WHERE name = $1 LIMIT 1",
      name);

  auto row = result[0];
  int count = row[0].as<int>();
  return count > 0;
}

string people_repository::create_pearson(Pearson &pearson)
{
  if (name_exists(pearson.name))
  {
    throw PearsonAlreadyExists();
  }

  auto redis_connection = redis_client::connect();
  string id = generate_uuid();
  pearson.id = id;
  string redis_key = "people:" + id;

  string serialized_pearson = json_dto::to_json(pearson);
  cout << "Serialized pearson: " << serialized_pearson << endl;
  redis_client::push_on_queue(redis_connection, "people", serialized_pearson.c_str());
  redis_client::set_key(redis_connection, redis_key.c_str(), serialized_pearson.c_str());
  redis_client::disconnect(redis_connection);

  return id;
}

void people_repository::create_cached_people()
{

  // Getting data from redis
  auto redis_connection = redis_client::connect();
  vector<string> people_as_strings = redis_client::get_all_from_queue(redis_connection, "people");
  vector<string> redis_keys;
  vector<Pearson> people;

  for (auto &person_as_string : people_as_strings)
  {
    Pearson person = json_dto::from_json<Pearson>(person_as_string);
    string redis_key = "people:" + person.id;
    redis_keys.push_back(redis_key);
    people.push_back(person);
  }

  cout << "Inserting " << people.size() << " people into database" << endl;
  // Inserting data into database
  auto connection = database_client::connect();
  pqxx::work txn(connection);

  ostringstream sql;

  sql << "INSERT INTO people (id, name, nickname, birth_date, stack_as_string) VALUES ";

  for (auto it = people.begin(); it != people.end(); ++it)
  {
    const auto &person = *it;
    string stack_as_string = person.stack ? stack_to_string(*person.stack) : "";
    sql << "("
        << txn.quote(person.id) << ","
        << txn.quote(person.name) << ","
        << txn.quote(person.nickname) << ","
        << txn.quote(person.birth_date) << ","
        << txn.quote(stack_as_string)
        << ")";

    if (next(it) != people.end())
    {
      sql << ",";
    }
  }

  cout << "Executing query: " << sql.str() << endl;
  txn.exec0(sql.str());

  txn.commit();

  // Removing data from redis
  for (auto &redis_key : redis_keys)
  {
    redis_client::delete_key(redis_connection, redis_key.c_str());
  }
  redis_client::delete_queue(redis_connection, "people");
  redis_client::disconnect(redis_connection);
}

Pearson people_repository::get_pearson(string &id)
{
  auto redis_connection = redis_client::connect();
  string redis_key = "people:" + id;
  string serialized_pearson = redis_client::get_key(redis_connection, redis_key.c_str());
  redis_client::disconnect(redis_connection);

  if (serialized_pearson != "")
  {
    Pearson pearson = json_dto::from_json<Pearson>(serialized_pearson);
    return pearson;
  }

  auto connection = database_client::connect();
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
  auto redis_connection = redis_client::connect();
  vector<string> people_as_strings = redis_client::get_all_from_queue(redis_connection, "people");
  vector<Pearson> people;

  for (auto &person_as_string : people_as_strings)
  {
    Pearson person = json_dto::from_json<Pearson>(person_as_string);
    string stack_as_string = person.stack ? stack_to_string(*person.stack) : "";
    if (person.name.find(term) != string::npos || person.nickname.find(term) != string::npos || stack_as_string.find(term) != string::npos)
    {
      people.push_back(person);
    }
  }
  redis_client::disconnect(redis_connection);

  auto connection = database_client::connect();
  pqxx::work txn(connection);

  pqxx::result result = txn.exec_params(
      "SELECT id, name, nickname, birth_date, stack_as_string FROM people WHERE name LIKE $1 OR nickname LIKE $1 OR stack_as_string LIKE $1",
      "%" + term + "%");

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
  auto redis_connection = redis_client::connect();
  int count = redis_client::get_queue_size(redis_connection, "people");
  redis_client::disconnect(redis_connection);

  auto connection = database_client::connect();
  pqxx::work txn(connection);

  pqxx::result result = txn.exec("SELECT COUNT(*) FROM people");
  auto row = result[0];
  count += row[0].as<int>();
  return count;
}
