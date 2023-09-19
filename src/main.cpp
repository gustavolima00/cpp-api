#include <iostream>

#include <restinio/all.hpp>
#include "requests/people_router.hpp"
#include <cstdlib>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include "repositories/people_repository.hpp"

#define DEFAULT_SERVER_PORT "8080"

using router_t = restinio::router::express_router_t<>;

auto server_handler()
{
  auto router = std::make_unique<router_t>();
  people_router::register_routes(router);

  return router;
}

std::mutex mtx;                       // Mutex para evitar condições de corrida
std::atomic<bool> stop_worker(false); // Flag para parar o worker

void worker()
{
  auto last_time_checked = std::chrono::system_clock::now();

  while (!stop_worker)
  {                                         // Continue executando até que a flag stop_worker seja definida como true
    std::unique_lock<std::mutex> lock(mtx); // Trave o mutex para evitar condições de corrida

    auto current_time = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = current_time - last_time_checked;

    auto redis_connection = redis_client::connect();
    int queue_size = redis_client::get_queue_size(redis_connection, "people");
    redis_client::disconnect(redis_connection);

    if (queue_size >= 50 || elapsed_seconds.count() >= 30.0)
    {
      cout << "Creating cached people with queue size: " << queue_size << endl;
      people_repository::create_cached_people();            // Sua função para inserir dados no banco de dados
      last_time_checked = std::chrono::system_clock::now(); // Atualize o último horário de verificação
    }

    lock.unlock(); // Libere o mutex

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Espere um pouco antes de verificar novamente
  }
}

int main()
{
  using namespace std::chrono;
  const char *server_port = std::getenv("SERVER_PORT");
  if (server_port == nullptr)
  {
    server_port = DEFAULT_SERVER_PORT;
  }

  std::thread worker_thread(worker);

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
