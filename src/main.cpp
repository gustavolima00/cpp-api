#include "api_handlers.h"

int main()
{
  std::cout << "Starting API...\n";

  api::initialize();

  api::handle_request("GET", "/users");

  return 0;
}
