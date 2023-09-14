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
  template <typename RESP>
  static RESP init_response(RESP resp, std::string content_type = "application/json")
  {
    resp
        .append_header("Server", "RESTinio sample server /v.0.6")
        .append_header_date_field()
        .append_header("Content-Type", content_type);

    return resp;
  }

  template <typename RESP>
  static void set_response_status(RESP &resp, const ResponseStatus &status)
  {
    switch (status)
    {
    case ResponseStatus::OK:
      resp.header().status_line(restinio::status_ok());
      break;
    case ResponseStatus::NOT_FOUND:
      resp.header().status_line(restinio::status_not_found());
      break;
    case ResponseStatus::BAD_REQUEST:
      resp.header().status_line(restinio::status_bad_request());
      break;
    case ResponseStatus::INTERNAL_SERVER_ERROR:
      resp.header().status_line(restinio::status_internal_server_error());
      break;
    default:
      resp.header().status_line(restinio::status_not_implemented());
      break;
    }
  }

  template <typename RESP, typename ResponseType>
  static void set_response_body_as_json(RESP &resp, ResponseType &response_body)
  {
    std::string response_json = json_dto::to_json(response_body);
    resp.set_body(response_json);
  }
};
