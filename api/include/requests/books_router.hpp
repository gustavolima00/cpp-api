#pragma once

#include <iostream>

#include <restinio/all.hpp>
#include "requests/books_requests_handler.hpp"

namespace rr = restinio::router;
using namespace std::placeholders;
using router_t = rr::express_router_t<>;

class BooksRouter
{
public:
  BooksRouter();
  static BooksRouter &getInstance();

  // Delete copy constructor and assignment operator
  BooksRouter(BooksRouter const &) = delete;
  void operator=(BooksRouter const &) = delete;

  void register_routes(std::unique_ptr<router_t> &router);
};
