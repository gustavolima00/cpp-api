#include "api_handlers.h"

namespace api
{

  void initialize()
  {
    std::cout << "API Initialized.\n";
  }

  void handle_request(const std::string &method, const std::string &path)
  {
    std::cout << "Handling " << method << " request at " << path << ".\n";
  }

} // namespace api
