#include "requests/pessoas_requests_handler.hpp"

using namespace base_request_handler;

restinio::request_handling_status_t pessoas_requests_handler::on_pessoa_create(
    const restinio::request_handle_t &req, rr::route_params_t)
{
  try
  {
    Pessoa pessoa = json_dto::from_json<Pessoa>(req->body());
    pessoas_repository::create_pessoa(pessoa);
    auto response = init_response(req->create_response(), "application/json");
    return return_as_json(response, pessoa);
  }
  catch (const std::exception &ex)
  {
    auto response = init_response(req->create_response(), "plain/text");
    return return_internal_server_error(response, ex);
  }
}

restinio::request_handling_status_t pessoas_requests_handler::on_pessoa_get(
    const restinio::request_handle_t &req, rr::route_params_t params)
{
  try
  {
    // From route :id
    auto id = restinio::cast_to<std::string>(params["id"]);
    cout << "id: " << id << endl;
    auto pessoa = pessoas_repository::get_pessoa(id);
    auto response = init_response(req->create_response(), "application/json");
    return return_as_json(response, pessoa);
  }
  catch (const std::exception &ex)
  {
    auto response = init_response(req->create_response(), "plain/text");
    return return_internal_server_error(response, ex);
  }
}

restinio::request_handling_status_t pessoas_requests_handler::on_pessoa_search(const restinio::request_handle_t &req, rr::route_params_t)
{
  try
  {
    const auto t_query = req->header().query();

    cout << "t_query: " << t_query << endl;
    string term = "";
    auto pessoas = pessoas_repository::search_pessoa(term);

    auto response = init_response(req->create_response(), "application/json");
    return return_as_json(response, pessoas);
  }
  catch (const std::exception &ex)
  {
    auto response = init_response(req->create_response(), "plain/text");
    return return_internal_server_error(response, ex);
  }
}

restinio::request_handling_status_t pessoas_requests_handler::on_pessoa_count(
    const restinio::request_handle_t &req, rr::route_params_t)
{
  try
  {
    auto count = pessoas_repository::count_pessoa();
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
