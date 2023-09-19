#include <iostream>

#include <restinio/all.hpp>
#include "requests/people_router.hpp"
#include <cstdlib>
#include "repositories/people_repository.hpp"

#define DEFAULT_SERVER_PORT "8080"

using router_t = restinio::router::express_router_t<>;

auto server_handler()
{
  auto router = std::make_unique<router_t>();
  people_router::register_routes(router);

  return router;
}

void print_people()
{
  string term = "";
  cout << "People count: " << people_repository::count_people() << endl;
  vector<Pearson> people = people_repository::search_people(term);
  for (auto &person : people)
  {
    cout << person.id << " " << person.name << " " << person.birth_date << endl;
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

  print_people();
  Pearson person;
  person.name = "João";
  person.nickname = "Joãozinho";
  person.birth_date = "1999-01-01";
  person.stack = {"C++", "C", "Python"};
  people_repository::create_pearson(person);
  print_people();

  person.name = "Maria";
  person.nickname = "Mariazinha";
  person.birth_date = "1999-01-01";
  person.stack = {"C++", "C", "Python"};
  people_repository::create_pearson(person);

  print_people();
  cout << "Creating cached people" << endl;
  people_repository::create_cached_people();
  print_people();

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
