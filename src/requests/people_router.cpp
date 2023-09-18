#include "requests/people_router.hpp"

void people_router::register_routes(std::unique_ptr<router_t> &router)
{
  // Handlers for '/' path.
  router->http_post("/pessoas", people_requests_handler::on_create_pearson);
  router->http_get("/pessoas/:id", people_requests_handler::on_get_pearson);

  // GET /people?t=[:termo da busca]
  router->http_get("/pessoas", people_requests_handler::on_search_people);
  router->http_get("/contagem-pessoas", people_requests_handler::on_count_people);
}
