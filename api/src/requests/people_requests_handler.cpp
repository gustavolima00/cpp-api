#include "requests/people_requests_handler.hpp"

using namespace base_request_handler;

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
    cout << "Error: " << ex.what() << endl;
    response.header().status_line(restinio::status_internal_server_error());
    return response.done();
  }
}

string get_term_string(const restinio::string_view_t query)
{
  string term;
  size_t i = 0;
  while (i < query.size() && query[i] != 't')
  {
    i++;
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
  try
  {
    const restinio::string_view_t t_query = req->header().query();
    string term = get_term_string(t_query);
    cout << "t_query: " << t_query << endl;
    cout << "term: " << term << endl;
    auto people = people_repository::search_people(term);

    auto response = init_response(req->create_response(), "application/json");
    return return_as_json(response, people);
  }
  catch (const std::exception &ex)
  {
    auto response = init_response(req->create_response(), "plain/text");
    return return_internal_server_error(response, ex);
  }
}

restinio::request_handling_status_t people_requests_handler::on_count_people(
    const restinio::request_handle_t &req, rr::route_params_t)
{
  try
  {
    auto count = people_repository::count_people();
    auto response = init_response(req->create_response(), "plain/text");
    response.set_body(std::to_string(count));
    return response.done();
  }
  catch (const std::exception &ex)
  {
    auto response = init_response(req->create_response(), "plain/text");
    return return_internal_server_error(response, ex);
  }
}
