#include "requests/books_router.hpp"

BooksRouter::BooksRouter() = default;

BooksRouter &BooksRouter::getInstance()
{
  static BooksRouter instance;
  return instance;
}

void BooksRouter::register_routes(std::unique_ptr<router_t> &router)
{
  // Handlers for '/' path.
  router->http_get("/", std::bind(&BooksRequestsHandler::on_books_list, &BooksRequestsHandler::getInstance(), _1, _2));
  router->http_post("/", std::bind(&BooksRequestsHandler::on_new_book, &BooksRequestsHandler::getInstance(), _1, _2));

  // Handler for '/author/:author' path.
  router->http_get("/author/:author", std::bind(&BooksRequestsHandler::on_author_get, &BooksRequestsHandler::getInstance(), _1, _2));

  // Handlers for '/:id' path.
  router->http_get(R"(/:id(\d+))", std::bind(&BooksRequestsHandler::on_book_get, &BooksRequestsHandler::getInstance(), _1, _2));
  router->http_put(R"(/:id(\d+))", std::bind(&BooksRequestsHandler::on_book_update, &BooksRequestsHandler::getInstance(), _1, _2));
  router->http_delete(R"(/:id(\d+))", std::bind(&BooksRequestsHandler::on_book_delete, &BooksRequestsHandler::getInstance(), _1, _2));
}
