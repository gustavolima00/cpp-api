#ifndef API_HANDLERS_H
#define API_HANDLERS_H

#include <iostream>
#include <string>

namespace api
{

  void initialize();

  void handle_request(const std::string &method, const std::string &path);

} // namespace api

#endif // API_HANDLERS_H
