#include "requests/books_requests_handler.hpp"

using namespace base_request_handler;

restinio::request_handling_status_t books_requests_handler::on_books_list(
    const restinio::request_handle_t &req, rr::route_params_t)
{
  try
  {
    auto books = books_repository::get_books();
    auto response = init_response(req->create_response(), "application/json");
    return return_as_json(response, books);
  }
  catch (const std::exception &ex)
  {
    auto response = init_response(req->create_response(), "plain/text");
    return return_internal_server_error(response, ex);
  }
}

restinio::request_handling_status_t books_requests_handler::on_book_get(const restinio::request_handle_t &req, rr::route_params_t params)
{
  try
  {
    const auto id = restinio::cast_to<std::uint32_t>(params["id"]);
    auto book = books_repository::get_book(id);

    auto response = init_response(req->create_response(), "application/json");
    return return_as_json(response, book);
  }
  catch (const std::exception &ex)
  {
    auto response = init_response(req->create_response(), "plain/text");
    return return_internal_server_error(response, ex);
  }
}

restinio::request_handling_status_t books_requests_handler::on_author_get(
    const restinio::request_handle_t &req, rr::route_params_t params)
{
  try
  {
    auto author = restinio::utils::unescape_percent_encoding(params["author"]);
    auto books = books_repository::get_by_author(author);

    auto response = init_response(req->create_response(), "application/json");
    return return_as_json(response, books);
  }
  catch (const std::exception &ex)
  {
    auto response = init_response(req->create_response(), "plain/text");
    return return_internal_server_error(response, ex);
  }
}

restinio::request_handling_status_t books_requests_handler::on_new_book(
    const restinio::request_handle_t &req, rr::route_params_t)
{
  try
  {
    Book new_book = json_dto::from_json<Book>(req->body());
    std::vector<Book> books;
    books.push_back(new_book);
    books_repository::add_books(books);
    auto response = init_response(req->create_response(), "application/json");
    return return_as_json(response, new_book);
  }
  catch (const std::exception &ex)
  {
    auto response = init_response(req->create_response(), "plain/text");
    return return_internal_server_error(response, ex);
  }
}

restinio::request_handling_status_t books_requests_handler::on_book_update(
    const restinio::request_handle_t &req, rr::route_params_t)
{
  try
  {
    Book book = json_dto::from_json<Book>(req->body());
    books_repository::update_book(book);
    auto response = init_response(req->create_response(), "application/json");
    return return_as_json(response, book);
  }
  catch (const std::exception &ex)
  {
    auto response = init_response(req->create_response(), "plain/text");
    return return_internal_server_error(response, ex);
  }
}

restinio::request_handling_status_t books_requests_handler::on_book_delete(
    const restinio::request_handle_t &req, rr::route_params_t params)
{
  try
  {
    const auto id = restinio::cast_to<std::uint32_t>(params["id"]);
    books_repository::delete_book(id);
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
