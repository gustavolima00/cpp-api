#include "models/pearson.hpp"

Pearson::Pearson() = default;

Pearson::Pearson(const string &id, const string &name, const string &nickname, const string &birth_date, vector<string> &stack)
{
    this->id = id;
    this->name = name;
    this->nickname = nickname;
    this->birth_date = birth_date;
    this->stack = stack;
}
