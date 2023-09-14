#include <iostream>

#include <restinio/all.hpp>
#include <json_dto/pub.hpp>
#include "models/book.hpp"
#include "repositories/books_repository.hpp"
#include "requests/books_requests_handler.hpp"
#include <functional>

using book_collection_t = std::vector<Book>;

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

auto server_handler()
{
  auto router = std::make_unique<router_t>();
  auto handler = std::make_shared<BooksRequestsHandler>();

  auto by = [&](auto method)
  {
    using namespace std::placeholders;
    return std::bind(method, handler, _1, _2);
  };

  auto method_not_allowed = [](const auto &req, auto)
  {
    return req->create_response(restinio::status_method_not_allowed())
        .connection_close()
        .done();
  };

  // Handlers for '/' path.
  router->http_get("/", by(&BooksRequestsHandler::on_books_list));
  router->http_post("/", by(&BooksRequestsHandler::on_new_book));

  // Disable all other methods for '/'.
  router->add_handler(
      restinio::router::none_of_methods(
          restinio::http_method_get(), restinio::http_method_post()),
      "/", method_not_allowed);

  // Handler for '/author/:author' path.
  router->http_get("/author/:author", by(&BooksRequestsHandler::on_author_get));

  // Disable all other methods for '/author/:author'.
  router->add_handler(
      restinio::router::none_of_methods(restinio::http_method_get()),
      "/author/:author", method_not_allowed);

  // Handlers for '/:id' path.
  router->http_get(
      R"(/:id(\d+))",
      by(&BooksRequestsHandler::on_book_get));
  router->http_put(
      R"(/:id(\d+))",
      by(&BooksRequestsHandler::on_book_update));
  router->http_delete(
      R"(/:id(\d+))",
      by(&BooksRequestsHandler::on_book_delete));

  // Disable all other methods for '/:id'.
  router->add_handler(
      restinio::router::none_of_methods(
          restinio::http_method_get(),
          restinio::http_method_post(),
          restinio::http_method_delete()),
      R"(/:id(\d+))", method_not_allowed);

  return router;
}

int main()
{
  using namespace std::chrono;

  try
  {
    using traits_t =
        restinio::traits_t<
            restinio::asio_timer_manager_t,
            restinio::single_threaded_ostream_logger_t,
            router_t>;

    restinio::run(
        restinio::on_this_thread<traits_t>()
            .address("0.0.0.0")
            .request_handler(server_handler())
            .read_next_http_message_timelimit(10s)
            .write_http_response_timelimit(1s)
            .handle_request_timeout(1s));
  }
  catch (const std::exception &ex)
  {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
  }

  return 0;
}
