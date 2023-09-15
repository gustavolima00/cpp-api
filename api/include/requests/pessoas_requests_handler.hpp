#pragma once

#include <iostream>

#include <restinio/all.hpp>
#include <json_dto/pub.hpp>
#include "models/pessoa.hpp"
#include "repositories/pessoas_repository.hpp"
#include "requests/base_request_handler.hpp"
using namespace std;

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

namespace pessoas_requests_handler
{
    restinio::request_handling_status_t on_pessoa_create(
        const restinio::request_handle_t &req, rr::route_params_t);

    restinio::request_handling_status_t on_pessoa_get(const restinio::request_handle_t &req, rr::route_params_t params);

    restinio::request_handling_status_t on_pessoa_search(const restinio::request_handle_t &req, rr::route_params_t params);

    restinio::request_handling_status_t on_pessoa_count(const restinio::request_handle_t &req, rr::route_params_t params);
};
