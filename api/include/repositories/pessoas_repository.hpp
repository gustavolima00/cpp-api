#pragma once

#include "models/pessoa.hpp"
#include <vector>
#include <pqxx/pqxx>
using namespace std;

namespace pessoas_repository
{
  void create_pessoa(const Pessoa &pessoa);
  Pessoa get_pessoa(string &id);
  vector<Pessoa> search_pessoa(string &term);
  int count_pessoa();
};
