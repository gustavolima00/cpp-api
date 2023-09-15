#pragma once

#include "models/book.hpp"
#include <vector>
#include <pqxx/pqxx>

using book_collection_t = std::vector<Book>;

namespace books_repository
{
  book_collection_t get_books();
  Book get_book(std::uint32_t id);
  void add_books(const book_collection_t &books);
  void update_book(const Book &book);
  void delete_book(std::uint32_t id);
  book_collection_t get_by_author(const std::string &author);
};
