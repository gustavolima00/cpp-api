#include "requests/books_router.hpp"

void books_router::register_routes(std::unique_ptr<router_t> &router)
{
  // Handlers for '/' path.
  router->http_get("/", books_requests_handler::on_books_list);
  router->http_post("/", books_requests_handler::on_new_book);

  // Handler for '/author/:author' path.
  router->http_get("/author/:author", books_requests_handler::on_author_get);

  // Handlers for '/:id' path.
  router->http_get(R"(/:id(\d+))", books_requests_handler::on_book_get);
  router->http_put(R"(/:id(\d+))", books_requests_handler::on_book_update);
  router->http_delete(R"(/:id(\d+))", books_requests_handler::on_book_delete);
}
