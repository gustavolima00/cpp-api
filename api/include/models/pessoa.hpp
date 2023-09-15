#pragma once

#include <string>
#include <json_dto/pub.hpp>
using namespace std;

class Pessoa
{
public:
  Pessoa();
  Pessoa(const string &id, const string &nome, const string &apelido, const string &nascimento, vector<string> stack);

  template <typename JSON_IO>
  void json_io(JSON_IO &io)
  {
    io & json_dto::mandatory("nome", nome) & json_dto::mandatory("apelido", apelido) & json_dto::mandatory("nascimento", nascimento) & json_dto::optional("stack", stack, vector<string>()) & json_dto::optional("id", id, "");
  }

  string id;
  string nome;
  string apelido;
  string nascimento;
  vector<string> stack;
};
