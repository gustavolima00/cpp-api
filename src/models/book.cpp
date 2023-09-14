#include <json_dto/pub.hpp>
#include "models/book.hpp"
#include <string>

Book::Book() = default;

Book::Book(const std::string &_author, const std::string &_title)
    : author(_author), title(_title)
{
}
