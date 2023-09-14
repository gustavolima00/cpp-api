#pragma once

#include <iostream>

#include <restinio/all.hpp>
#include <json_dto/pub.hpp>
#include "models/book.hpp"
#include "repositories/books_repository.hpp"
#include "requests/base_request_handler.hpp"

using book_collection_t = std::vector<Book>;

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

class BooksRequestsHandler : BaseRequestHandler
{
public:
  BooksRequestsHandler() {}

  static BooksRequestsHandler &getInstance()
  {
    static BooksRequestsHandler instance;
    return instance;
  }

  BooksRequestsHandler(const BooksRequestsHandler &) = delete;
  BooksRequestsHandler(BooksRequestsHandler &&) = delete;

  auto on_books_list(
      const restinio::request_handle_t &req, rr::route_params_t) const
  {
    try
    {
      auto books = BooksRepository::getInstance().get_books();
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
      auto book = BooksRepository::getInstance().get_book(id);

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
      auto books = BooksRepository::getInstance().get_by_author(author);

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
      BooksRepository::getInstance().add_book(new_book);
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
      const restinio::request_handle_t &req, rr::route_params_t)
  {
    try
    {
      Book book = json_dto::from_json<Book>(req->body());
      BooksRepository::getInstance().update_book(book);
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
      BooksRepository::getInstance().delete_book(id);
      auto response = init_response(req->create_response(), "text/plain");
      set_response_status(response, ResponseStatus::OK);
      return response.done();
    }
    catch (const std::exception &ex)
    {
      auto response = init_response(req->create_response(), "plain/text");
      return return_internal_server_error(response, ex);
    }
  }
};
