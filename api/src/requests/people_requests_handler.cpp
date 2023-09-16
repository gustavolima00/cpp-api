#include "requests/people_requests_handler.hpp"

using namespace base_request_handler;

people_requests_handler::MissingQueryParamException::MissingQueryParamException() {}

const char *people_requests_handler::MissingQueryParamException::what()
{
  return "Missing query param";
}

restinio::request_handling_status_t people_requests_handler::on_create_pearson(
    const restinio::request_handle_t &req, rr::route_params_t)
{
  auto response = req->create_response();

  try
  {
    Pearson pearson = json_dto::from_json<Pearson>(req->body());
    string id = people_repository::create_pearson(pearson);
    response.header().status_line(restinio::status_created());
    return response.append_header("Location", "/pessoas/" + id).done();
  }
  catch (const std::exception &ex)
  {
    response.header().status_line(restinio::status_unprocessable_entity());
    return response.done();
  }
}

restinio::request_handling_status_t people_requests_handler::on_get_pearson(
    const restinio::request_handle_t &req, rr::route_params_t params)
{
  auto response = req->create_response();
  try
  {
    // From route :id
    auto id = restinio::cast_to<std::string>(params["id"]);
    auto pearson = people_repository::get_pearson(id);

    response.header().status_line(restinio::status_ok());

    string response_json = json_dto::to_json(pearson);
    response.set_body(response_json);
    return response.append_header("Content-Type", "application/json").done();
  }
  catch (const people_repository::PearsonNotFound &ex)
  {
    response.header().status_line(restinio::status_not_found());
    return response.done();
  }
  catch (const std::exception &ex)
  {
    response.header().status_line(restinio::status_internal_server_error());
    return response.done();
  }
}

string get_term_string(const restinio::string_view_t query)
{
  string term;
  size_t i = 0;
  bool has_term = false;
  while (i < query.size() && query[i] != 't')
  {
    has_term = true;
    i++;
  }
  if (!has_term)
  {
    throw people_requests_handler::MissingQueryParamException();
  }
  i += 2; // t=
  while (i < query.size() && query[i] != '&')
  {
    term += query[i];
    i++;
  }
  return url_decode(term);
}

restinio::request_handling_status_t people_requests_handler::on_search_people(const restinio::request_handle_t &req, rr::route_params_t)
{
  auto response = req->create_response();
  try
  {
    const restinio::string_view_t t_query = req->header().query();
    string term = get_term_string(t_query);
    auto people = people_repository::search_people(term);

    string response_json = json_dto::to_json(people);
    response.set_body(response_json);
    return response.append_header("Content-Type", "application/json").done();
  }
  catch (const people_requests_handler::MissingQueryParamException &ex)
  {
    response.header().status_line(restinio::status_bad_request());
    return response.done();
  }
  catch (const std::exception &ex)
  {
    response.header().status_line(restinio::status_internal_server_error());
    return response.done();
  }
}

restinio::request_handling_status_t people_requests_handler::on_count_people(
    const restinio::request_handle_t &req, rr::route_params_t)
{
  auto response = req->create_response();
  try
  {
    auto count = people_repository::count_people();
    response.set_body(std::to_string(count));
    return response.append_header("Content-Type", "text/plain").done();
  }
  catch (const std::exception &ex)
  {
    response.header().status_line(restinio::status_internal_server_error());
    return response.done();
  }
}
