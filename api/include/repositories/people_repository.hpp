#pragma once

#include "models/pearson.hpp"
#include "database/database.hpp"
#include <vector>
using namespace std;

namespace people_repository
{
  string create_pearson(const Pearson &pearson);
  Pearson get_pearson(string &id);
  vector<Pearson> search_people(string &term);
  int count_people();

  class PearsonNotFound : public std::exception
  {
  public:
    PearsonNotFound();
     const char* what();
  };
};
