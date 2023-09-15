#include "models/pessoa.hpp"

Pessoa::Pessoa() = default;

Pessoa::Pessoa(const string &id, const string &nome, const string &apelido, const string &nascimento, vector<string> stack)
{
    this->id = id;
    this->nome = nome;
    this->apelido = apelido;
    this->nascimento = nascimento;
    this->stack = stack;
}
