#pragma once

#include <string>
#include <json_dto/pub.hpp>
using namespace std;

class Pearson
{
public:
  Pearson();
  Pearson(const string &id, const string &name, const string &nickname, const string &birth_date, vector<string>& stack);

  template <typename JSON_IO>
  void json_io(JSON_IO &io)
  {
    io & json_dto::mandatory("nome", name) & json_dto::mandatory("apelido", nickname) & json_dto::mandatory("nascimento", birth_date) & json_dto::optional("stack", stack, vector<string>()) & json_dto::optional("id", id, "");
  }

  string id;
  string name;
  string nickname;
  string birth_date;
  vector<string> stack;
};
