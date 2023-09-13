#pragma once

#include "models/book.hpp"
#include <vector>
#include <memory>
#include <unordered_map>
#include <vector>

using book_collection_t = std::vector<Book>;

class BooksRepository
{
public:
  static BooksRepository &getInstance()
  {
    static BooksRepository instance;
    return instance;
  }

  // Delete copy constructor and assignment operator
  BooksRepository(BooksRepository const &) = delete;
  void operator=(BooksRepository const &) = delete;

  book_collection_t get_books() const
  {
    std::vector<Book> books;
    for (auto &kv : books_map)
    {
      books.push_back(kv.second);
    }
    return books;
  }

  Book get_book(std::uint32_t id) const
  {
    return books_map.at(id);
  }

  void add_book(Book book)
  {
    book.id = next_id++;
    books_map[book.id] = book;
  }

  void update_book(const Book &book)
  {
    books_map[book.id] = book;
  }

  void delete_book(std::uint32_t id)
  {
    books_map.erase(id);
  }

  book_collection_t get_by_author(const std::string &author) const
  {
    std::vector<Book> books;
    for (auto &kv : books_map)
    {
      if (kv.second.author == author)
      {
        books.push_back(kv.second);
      }
    }
    return books;
  }

  BooksRepository() = default;

private:
  std::unordered_map<uint32_t, Book> books_map;
  uint32_t next_id = 1;
};
