#include "models/book.hpp"

Book::Book() = default;

Book::Book(const std::string &_author, const std::string &_title)
    : author(_author), title(_title)
{
}
