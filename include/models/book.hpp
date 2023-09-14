#pragma once

#include <string>
#include <json_dto/pub.hpp>

class Book
{
public:
  Book();
  Book(const std::string &author, const std::string &title);

  template <typename JSON_IO>
  void json_io(JSON_IO &io)
  {
    io &json_dto::mandatory("author", author) & json_dto::mandatory("title", title) & json_dto::optional("id", id, 0);
  }

  std::string author;
  std::string title;
  uint32_t id;
};
