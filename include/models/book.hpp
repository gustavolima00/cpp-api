#pragma once

#include <string>
#include <json_dto/pub.hpp>

class Book
{
public:
  Book() = default;

  Book(const std::string &_author, const std::string &_title)
      : author(_author), title(_title)
  {
  }

  template <typename JSON_IO>
  void json_io(JSON_IO &io)
  {
    io &json_dto::mandatory("author", author) & json_dto::mandatory("title", title);
  }

  std::string author;
  std::string title;
};
