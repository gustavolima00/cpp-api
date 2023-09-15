#include "repositories/books_repository.hpp"

const std::string CONNECTION_STRING = "postgresql://postgres:postgres@localhost:5432/postgres";
// const std::string CONNECTION_STRING = "postgresql://postgres:postgres@database:5432/postgres";

book_collection_t from_result_to_books(pqxx::result &r)
{
  std::vector<Book> books;
  for (auto row : r)
  {
    Book book;
    book.id = row[0].as<std::uint32_t>();
    book.title = row[1].as<std::string>();
    book.author = row[2].as<std::string>();
    books.push_back(book);
  }
  return books;
}

book_collection_t books_repository::get_books()
{
  pqxx::connection c(CONNECTION_STRING);
  pqxx::work w(c);
  pqxx::result r = w.exec("SELECT id, title, author FROM books");

  std::vector<Book> books = from_result_to_books(r);
  w.commit();
  return books;
}

Book books_repository::get_book(std::uint32_t id)
{
  pqxx::connection c(CONNECTION_STRING);
  pqxx::work w(c);
  pqxx::result r = w.exec("SELECT id, title, author FROM books WHERE id = " + w.quote(id));
  std::vector<Book> books = from_result_to_books(r);
  w.commit();
  return books.at(0);
}

void books_repository::add_books(const book_collection_t &books)
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

void books_repository::update_book(const Book &book)
{
  pqxx::connection c(CONNECTION_STRING);
  pqxx::work w(c);
  std::string sql = "UPDATE books SET title = '" + w.esc(book.title) + "', author = '" + w.esc(book.author) + "' WHERE id = " + w.quote(book.id);
  w.exec(sql);
  w.commit();
}

void books_repository::delete_book(std::uint32_t id)
{
  pqxx::connection c(CONNECTION_STRING);
  pqxx::work w(c);
  std::string sql = "DELETE FROM books WHERE id = " + w.quote(id);
  w.exec(sql);
  w.commit();
}

book_collection_t books_repository::get_by_author(const std::string &author)
{
  pqxx::connection c(CONNECTION_STRING);
  pqxx::work w(c);
  pqxx::result r = w.exec("SELECT id, title, author FROM books WHERE author = '" + w.esc(author) + "'");
  std::vector<Book> books = from_result_to_books(r);
  w.commit();
  return books;
}
