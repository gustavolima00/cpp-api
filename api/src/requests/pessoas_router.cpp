#include "requests/pessoas_router.hpp"

void pessoas_router::register_routes(std::unique_ptr<router_t> &router)
{
  // Handlers for '/' path.
  router->http_post("/pessoas", pessoas_requests_handler::on_pessoa_create);
  router->http_get("/pessoas/:id", pessoas_requests_handler::on_pessoa_get);

  // GET /pessoas?t=[:termo da busca]
  router->http_get("/pessoas", pessoas_requests_handler::on_pessoa_search);
  router->http_get("/contagem-pessoas", pessoas_requests_handler::on_pessoa_count);
}
