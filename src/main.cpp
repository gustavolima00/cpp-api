#include <iostream>

#include <restinio/all.hpp>
#include "requests/people_router.hpp"
#include <cstdlib>
#include "clients/redis_client.hpp"

#define DEFAULT_SERVER_PORT "8080"

using router_t = restinio::router::express_router_t<>;

auto server_handler()
{
  auto router = std::make_unique<router_t>();
  people_router::register_routes(router);

  return router;
}

int main()
{
  using namespace std::chrono;
  const char *server_port = std::getenv("SERVER_PORT");
  if (server_port == nullptr)
  {
    server_port = DEFAULT_SERVER_PORT;
  }

  try
  {
    using traits_t =
        restinio::traits_t<
            restinio::asio_timer_manager_t,
            restinio::single_threaded_ostream_logger_t,
            router_t>;

    restinio::run(
        restinio::on_this_thread<traits_t>()
            .address("0.0.0.0")
            .port(std::atoi(server_port))
            .request_handler(server_handler())
            .read_next_http_message_timelimit(3600s)
            .write_http_response_timelimit(3600s)
            .handle_request_timeout(3600s));
  }
  catch (const std::exception &ex)
  {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
  }

  return 0;
}
