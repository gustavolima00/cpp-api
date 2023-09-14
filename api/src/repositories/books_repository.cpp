#include "repositories/books_repository.hpp"
#include <pqxx/pqxx>

const std::string CONNECTION_STRING = "postgresql://postgres:postgres@localhost:5432/gustavolima";
// const std::string CONNECTION_STRING = "postgresql://postgres:postgres@database:5432/postgres";

BooksRepository &BooksRepository::getInstance()
{
  static BooksRepository instance;
  return instance;
}

book_collection_t BooksRepository::get_books() const
{
  std::vector<Book> books;
  try
  {
    pqxx::connection c(CONNECTION_STRING);
    pqxx::work w(c);
    pqxx::result r = w.exec("SELECT id, title, author FROM books");

    for (auto row : r)
    {
      Book book;
      book.id = row["id"].as<std::uint32_t>();
      book.title = row["title"].as<std::string>();
      book.author = row["author"].as<std::string>();
      books.push_back(book);
    }
    w.commit();
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  return books;
}

Book BooksRepository::get_book(std::uint32_t id) const
{
  return books_map.at(id);
}

void BooksRepository::add_books(const book_collection_t &books)
{
  try
  {
    pqxx::connection c(CONNECTION_STRING);
    pqxx::work w(c);

    for (const auto &book : books)
    {
      std::string sql = "INSERT INTO books (title, author) VALUES ('" + w.esc(book.title) + "', '" + w.esc(book.author) + "');";
      w.exec(sql);
    }
    w.commit();
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
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
