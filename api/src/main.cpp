#include <iostream>

#include <restinio/all.hpp>
#include "requests/people_router.hpp"
// #define PORT 8080
#define PORT 80

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

auto server_handler()
{
  auto router = std::make_unique<router_t>();
  people_router::register_routes(router);

  return router;
}

int main()
{
  using namespace std::chrono;

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
            .port(PORT)
            .request_handler(server_handler())
            .read_next_http_message_timelimit(10s)
            .write_http_response_timelimit(1s)
            .handle_request_timeout(1s));
  }
  catch (const std::exception &ex)
  {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
  }

  return 0;
}
