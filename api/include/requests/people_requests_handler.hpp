#pragma once

#include <iostream>

#include <restinio/all.hpp>
#include <json_dto/pub.hpp>
#include "models/pearson.hpp"
#include "repositories/people_repository.hpp"
#include "requests/base_request_handler.hpp"
using namespace std;

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

namespace people_requests_handler
{
    restinio::request_handling_status_t on_create_pearson(
        const restinio::request_handle_t &req, rr::route_params_t);

    restinio::request_handling_status_t on_get_pearson(const restinio::request_handle_t &req, rr::route_params_t params);

    restinio::request_handling_status_t on_search_people(const restinio::request_handle_t &req, rr::route_params_t params);

    restinio::request_handling_status_t on_count_people(const restinio::request_handle_t &req, rr::route_params_t params);
};
