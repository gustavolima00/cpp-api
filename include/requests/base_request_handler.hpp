#pragma once

#include <iostream>
#include <restinio/all.hpp>
#include <json_dto/pub.hpp>

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

enum ResponseStatus
{
  OK = 200,
  BAD_REQUEST = 400,
  NOT_FOUND = 404,
  METHOD_NOT_ALLOWED = 405,
  INTERNAL_SERVER_ERROR = 500
};

class BaseRequestHandler
{
public:
  explicit BaseRequestHandler() = default;

protected:
  template <typename ResponseBuilder>
  static ResponseBuilder init_response(ResponseBuilder response, std::string content_type = "plain/text")
  {
    response
        .append_header("Server", "RESTinio sample server /v.0.6")
        .append_header_date_field()
        .append_header("Content-Type", content_type);
    return response;
  }

  template <typename ResponseBuilder>
  static void set_response_status(ResponseBuilder &response, const ResponseStatus &status)
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

  template <typename ResponseBuilder, typename ResponseType>
  static void set_response_body_as_json(ResponseBuilder &response, ResponseType &response_body)
  {
    std::string response_json = json_dto::to_json(response_body);
    response.set_body(response_json);
  }

  template <typename ResponseBuilder, typename ResponseType>
  static auto return_as_json(ResponseBuilder &response, ResponseType &response_body, const ResponseStatus &status = ResponseStatus::OK)
  {
    set_response_status(response, status);
    set_response_body_as_json(response, response_body);
    return response.done();
  }

  template <typename ResponseBuilder>
  static auto return_internal_server_error(ResponseBuilder &response, const std::exception &exception)
  {
    set_response_status(response, ResponseStatus::INTERNAL_SERVER_ERROR);
    response.set_body(exception.what());
    return response.done();
  }
};
