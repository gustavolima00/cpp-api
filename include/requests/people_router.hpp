#pragma once

#include <iostream>

#include <restinio/all.hpp>
#include "requests/people_requests_handler.hpp"

namespace rr = restinio::router;
using namespace std::placeholders;
using router_t = rr::express_router_t<>;

namespace people_router
{
  void register_routes(std::unique_ptr<router_t> &router);
};
