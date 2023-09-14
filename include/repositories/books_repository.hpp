#pragma once

#include "models/book.hpp"
#include <vector>
#include <memory>
#include <unordered_map>
#include <cstdint>

using book_collection_t = std::vector<Book>;

class BooksRepository
{
public:
  static BooksRepository &getInstance();

  // Delete copy constructor and assignment operator
  BooksRepository(BooksRepository const &) = delete;
  void operator=(BooksRepository const &) = delete;

  book_collection_t get_books() const;
  Book get_book(std::uint32_t id) const;
  void add_book(Book book);
  void update_book(const Book &book);
  void delete_book(std::uint32_t id);
  book_collection_t get_by_author(const std::string &author) const;

  BooksRepository();

private:
  std::unordered_map<uint32_t, Book> books_map;
  uint32_t next_id = 1;
};
