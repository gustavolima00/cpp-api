#include "repositories/books_repository.hpp"

BooksRepository &BooksRepository::getInstance()
{
  static BooksRepository instance;
  return instance;
}

book_collection_t BooksRepository::get_books() const
{
  std::vector<Book> books;
  for (auto &kv : books_map)
  {
    books.push_back(kv.second);
  }
  return books;
}

Book BooksRepository::get_book(std::uint32_t id) const
{
  return books_map.at(id);
}

void BooksRepository::add_book(Book book)
{
  book.id = next_id++;
  books_map[book.id] = book;
}

void BooksRepository::update_book(const Book &book)
{
  books_map[book.id] = book;
}

void BooksRepository::delete_book(std::uint32_t id)
{
  books_map.erase(id);
}

book_collection_t BooksRepository::get_by_author(const std::string &author) const
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

BooksRepository::BooksRepository() = default;
