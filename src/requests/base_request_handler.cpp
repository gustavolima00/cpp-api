#include "requests/base_request_handler.hpp"

BaseRequestHandler::BaseRequestHandler() = default;

response_builder_t BaseRequestHandler::init_response(
    response_builder_t response,
    std::string content_type)
{
  response
      .append_header("Server", "RESTinio sample server /v.0.6")
      .append_header_date_field()
      .append_header("Content-Type", content_type);
  return response;
}

void BaseRequestHandler::set_response_status(response_builder_t &response, const ResponseStatus &status)
{
  switch (status)
  {
  case ResponseStatus::OK:
    response.header().status_line(restinio::status_ok());
    break;
  case ResponseStatus::NOT_FOUND:
    response.header().status_line(restinio::status_not_found());
    break;
  case ResponseStatus::BAD_REQUEST:
    response.header().status_line(restinio::status_bad_request());
    break;
  case ResponseStatus::INTERNAL_SERVER_ERROR:
    response.header().status_line(restinio::status_internal_server_error());
    break;
  default:
    response.header().status_line(restinio::status_not_implemented());
    break;
  }
}

restinio::request_handling_status_t BaseRequestHandler::return_internal_server_error(response_builder_t &response, const std::exception &exception)
{
  set_response_status(response, ResponseStatus::INTERNAL_SERVER_ERROR);
  response.set_body(exception.what());
  return response.done();
}
