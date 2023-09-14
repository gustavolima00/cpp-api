#pragma once

#include <iostream>
#include <restinio/all.hpp>
#include <json_dto/pub.hpp>

using response_builder_t = restinio::response_builder_t<restinio::restinio_controlled_output_t>;

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
  BaseRequestHandler();

protected:
  static response_builder_t init_response(response_builder_t response, std::string content_type = "plain/text");

  static void set_response_status(response_builder_t &response, const ResponseStatus &status);

  template <typename DataType>
  static void set_response_body_as_json(response_builder_t &response, DataType &data)
  {
    std::string response_json = json_dto::to_json(data);
    response.set_body(response_json);
  }

  template <typename DataType>
  static restinio::request_handling_status_t return_as_json(response_builder_t &response, DataType &data, const ResponseStatus &status = ResponseStatus::OK)
  {
    set_response_status(response, status);
    set_response_body_as_json(response, data);
    return response.done();
  }
  static restinio::request_handling_status_t return_internal_server_error(response_builder_t &response, const std::exception &exception);
};
