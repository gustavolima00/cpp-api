#pragma once

#include "models/pearson.hpp"
#include "clients/database_client.hpp"
#include "clients/redis_client.hpp"
#include <vector>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
using namespace std;

namespace people_repository
{
  class PearsonNotFound : public std::exception
  {
  public:
    PearsonNotFound();
    const char *what() const noexcept;
  };

  class PearsonAlreadyExists : public std::exception
  {
  public:
    PearsonAlreadyExists();
    const char *what() const noexcept;
  };

  bool name_exists(string &name);
  string create_pearson(Pearson &pearson);
  void create_cached_people();
  Pearson get_pearson(string &id);
  vector<Pearson> search_people(string &term);
  int count_people();

};
