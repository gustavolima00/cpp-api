#pragma once

#include <iostream>

#include <restinio/all.hpp>
#include <json_dto/pub.hpp>
#include "models/book.hpp"
#include "repositories/books_repository.hpp"
#include "requests/base_request_handler.hpp"

using book_collection_t = std::vector<Book>;

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

class BooksRequestsHandler : BaseRequestHandler
{
public:
    BooksRequestsHandler();

    static BooksRequestsHandler &getInstance();

    BooksRequestsHandler(const BooksRequestsHandler &) = delete;
    BooksRequestsHandler(BooksRequestsHandler &&) = delete;

    restinio::request_handling_status_t on_books_list(
        const restinio::request_handle_t &req, rr::route_params_t) const;

    restinio::request_handling_status_t on_book_get(const restinio::request_handle_t &req, rr::route_params_t params);

    restinio::request_handling_status_t on_author_get(
        const restinio::request_handle_t &req, rr::route_params_t params);

    restinio::request_handling_status_t on_new_book(
        const restinio::request_handle_t &req, rr::route_params_t);

    restinio::request_handling_status_t on_book_update(
        const restinio::request_handle_t &req, rr::route_params_t);

    restinio::request_handling_status_t on_book_delete(
        const restinio::request_handle_t &req, rr::route_params_t params);
};
