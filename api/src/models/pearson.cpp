#include "models/pearson.hpp"

Pearson::Pearson() = default;

Pearson::Pearson(const string &id, const string &nome, const string &apelido, const string &nascimento, vector<string> stack)
{
    this->id = id;
    this->nome = nome;
    this->apelido = apelido;
    this->nascimento = nascimento;
    this->stack = stack;
}
