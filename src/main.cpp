#include <iostream>

#include <restinio/all.hpp>
#include "requests/people_router.hpp"
#include <cstdlib>
#include <hiredis/hiredis.h>

#define DEFAULT_SERVER_PORT "8080"

using router_t = restinio::router::express_router_t<>;

auto server_handler()
{
  auto router = std::make_unique<router_t>();
  people_router::register_routes(router);

  return router;
}

int main()
{
  using namespace std::chrono;
  const char *server_port = std::getenv("SERVER_PORT");
  if (server_port == nullptr)
  {
    server_port = DEFAULT_SERVER_PORT;
  }

  // Conectar ao servidor Redis rodando na porta 6379 do localhost
  redisContext *context = redisConnect("127.0.0.1", 6379);
  if (context == NULL || context->err)
  {
    if (context)
    {
      std::cout << "Erro: " << context->errstr << std::endl;
      // Libera o contexto
      redisFree(context);
    }
    else
    {
      std::cout << "Não foi possível alocar o contexto do redis" << std::endl;
    }
    return 1;
  }

  // Enviar comando SET
  redisReply *reply = (redisReply *)redisCommand(context, "SET %s %s", "chave", "valor");
  // Verificar se o comando foi bem-sucedido
  if (reply->type == REDIS_REPLY_STATUS)
  {
    std::cout << "SET realizado: " << reply->str << std::endl;
  }
  // Liberar o objeto de resposta
  freeReplyObject(reply);

  // Enviar comando GET
  reply = (redisReply *)redisCommand(context, "GET chave");
  if (reply->type == REDIS_REPLY_STRING)
  {
    std::cout << "GET chave: " << reply->str << std::endl;
  }
  freeReplyObject(reply);

  // Fechar a conexão e liberar o contexto
  redisFree(context);

  return 0;

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
