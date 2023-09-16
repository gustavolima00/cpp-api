#include "requests/base_request_handler.hpp"

response_builder_t base_request_handler::init_response(
    response_builder_t response,
    string content_type)
{
  response
      .append_header("Server", "RESTinio sample server /v.0.6")
      .append_header_date_field()
      .append_header("Content-Type", content_type);
  return response;
}

void base_request_handler::set_response_status(response_builder_t &response, const ResponseStatus &status)
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

restinio::request_handling_status_t base_request_handler::return_internal_server_error(response_builder_t &response, const exception &exception)
{
  set_response_status(response, ResponseStatus::INTERNAL_SERVER_ERROR);
  response.set_body(exception.what());
  return response.done();
}

string base_request_handler::url_decode(const string &str)
{
  ostringstream unescaped;
  for (size_t i = 0; i < str.size(); ++i)
  {
    if (str[i] == '%' && i + 2 < str.size())
    {
      int value;
      istringstream is(str.substr(i + 1, 2));
      if (is >> hex >> value)
      {
        unescaped << static_cast<char>(value);
        i += 2;
      }
      else
      {
        return "";
      }
    }
    else if (str[i] == '+')
    {
      unescaped << ' ';
    }
    else
    {
      unescaped << str[i];
    }
  }
  return unescaped.str();
}