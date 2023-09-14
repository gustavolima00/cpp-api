#pragma once

#include <iostream>

#include <restinio/all.hpp>
#include "requests/books_requests_handler.hpp"

using book_collection_t = std::vector<Book>;

namespace rr = restinio::router;
using namespace std::placeholders;
using router_t = rr::express_router_t<>;

class BooksRouter
{
public:
  BooksRouter() {}

  // Delete copy constructor and assignment operator
  BooksRouter(BooksRouter const &) = delete;
  void operator=(BooksRouter const &) = delete;

  static BooksRouter &getInstance()
  {
    static BooksRouter instance;
    return instance;
  }

  void register_routes(std::unique_ptr<router_t> &router)
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
  int next_id = 1;
};
