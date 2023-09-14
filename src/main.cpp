#include <iostream>

#include <restinio/all.hpp>
#include <json_dto/pub.hpp>
#include "models/book.hpp"
#include "repositories/books_repository.hpp"
#include "requests/base_request_handler.hpp"
#include <functional>

using book_collection_t = std::vector<Book>;

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

class books_handler_t : BaseRequestHandler
{
public:
  explicit books_handler_t()
  {
    BooksRepository &books_repository = BooksRepository::getInstance();
  }

  books_handler_t(const books_handler_t &) = delete;
  books_handler_t(books_handler_t &&) = delete;

  auto on_books_list(
      const restinio::request_handle_t &req, rr::route_params_t) const
  {
    try
    {
      auto books = books_repository.get_books();
      auto response = init_response(req->create_response(), "application/json");
      return return_as_json(response, books);
    }
    catch (const std::exception &ex)
    {
      auto response = init_response(req->create_response(), "plain/text");
      return return_internal_server_error(response, ex);
    }
  }

  auto on_book_get(const restinio::request_handle_t &req, rr::route_params_t params)
  {
    try
    {
      const auto id = restinio::cast_to<std::uint32_t>(params["id"]);
      auto book = books_repository.get_book(id);

      auto response = init_response(req->create_response(), "application/json");
      return return_as_json(response, book);
    }
    catch (const std::exception &ex)
    {
      auto response = init_response(req->create_response(), "plain/text");
      return return_internal_server_error(response, ex);
    }
  }

  auto on_author_get(
      const restinio::request_handle_t &req, rr::route_params_t params)
  {
    try
    {
      auto author = restinio::utils::unescape_percent_encoding(params["author"]);
      auto books = books_repository.get_by_author(author);

      auto response = init_response(req->create_response(), "application/json");
      return return_as_json(response, books);
    }
    catch (const std::exception &ex)
    {
      auto response = init_response(req->create_response(), "plain/text");
      return return_internal_server_error(response, ex);
    }
  }

  auto on_new_book(
      const restinio::request_handle_t &req, rr::route_params_t)
  {
    try
    {
      Book new_book = json_dto::from_json<Book>(req->body());
      books_repository.add_book(new_book);
      auto response = init_response(req->create_response(), "application/json");
      return return_as_json(response, new_book);
    }
    catch (const std::exception &ex)
    {
      auto response = init_response(req->create_response(), "plain/text");
      return return_internal_server_error(response, ex);
    }
  }

  auto on_book_update(
      const restinio::request_handle_t &req, rr::route_params_t params)
  {
    try
    {
      const auto id = restinio::cast_to<std::uint32_t>(params["id"]);
      Book book = json_dto::from_json<Book>(req->body());
      books_repository.update_book(book);
      auto response = init_response(req->create_response(), "application/json");
      return return_as_json(response, book);
    }
    catch (const std::exception &ex)
    {
      auto response = init_response(req->create_response(), "plain/text");
      return return_internal_server_error(response, ex);
    }
  }

  auto on_book_delete(
      const restinio::request_handle_t &req, rr::route_params_t params)
  {
    try
    {
      const auto id = restinio::cast_to<std::uint32_t>(params["id"]);
      books_repository.delete_book(id);
    }
    catch (const std::exception &ex)
    {
      auto response = init_response(req->create_response(), "plain/text");
      return return_internal_server_error(response, ex);
    }
  }

private:
  BooksRepository books_repository;
};

auto server_handler()
{
  auto router = std::make_unique<router_t>();
  auto handler = std::make_shared<books_handler_t>();

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
  router->http_get("/", by(&books_handler_t::on_books_list));
  router->http_post("/", by(&books_handler_t::on_new_book));

  // Disable all other methods for '/'.
  router->add_handler(
      restinio::router::none_of_methods(
          restinio::http_method_get(), restinio::http_method_post()),
      "/", method_not_allowed);

  // Handler for '/author/:author' path.
  router->http_get("/author/:author", by(&books_handler_t::on_author_get));

  // Disable all other methods for '/author/:author'.
  router->add_handler(
      restinio::router::none_of_methods(restinio::http_method_get()),
      "/author/:author", method_not_allowed);

  // Handlers for '/:id' path.
  router->http_get(
      R"(/:id(\d+))",
      by(&books_handler_t::on_book_get));
  router->http_put(
      R"(/:id(\d+))",
      by(&books_handler_t::on_book_update));
  router->http_delete(
      R"(/:id(\d+))",
      by(&books_handler_t::on_book_delete));

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
