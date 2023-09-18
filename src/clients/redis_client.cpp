#include "clients/redis_client.hpp"

redis_client::RedisClientException::RedisClientException(const char *message) : message(message)
{
}

const char *redis_client::RedisClientException::what() const noexcept
{
  return this->message;
}

redisContext *redis_client::connect()
{
  const char *redis_host = std::getenv("REDIS_HOST");
  if (redis_host == nullptr)
    redis_host = REDIS_DEFAULT_HOST;
  const char *redis_port = std::getenv("REDIS_PORT");
  if (redis_port == nullptr)
    redis_port = REDIS_DEFAULT_PORT;

  redisContext *context = redisConnect(redis_host, atoi(redis_port));
  if (context == NULL || context->err)
  {
    if (context)
    {
      throw RedisClientException(context->errstr);
      redisFree(context);
    }
    else
    {
      throw RedisClientException("Não foi possível alocar o contexto do redis");
    }
  }

  return context;
}

void redis_client::set_key(redisContext *context, const char *key, const char *value)
{
  redisReply *reply = (redisReply *)redisCommand(context, "SET %s %s", key, value);
  if (reply->type != REDIS_REPLY_STATUS)
  {
    throw RedisClientException("Erro ao executar o comando SET");
  }
  freeReplyObject(reply);
}

string redis_client::get_key(redisContext *context, const char *key)
{
  redisReply *reply = (redisReply *)redisCommand(context, "GET %s", key);
  if (reply->type != REDIS_REPLY_STRING)
  {
    throw RedisClientException("Erro ao executar o comando GET");
  }
  string value(reply->str);
  freeReplyObject(reply);
  return value;
}

void redis_client::disconnect(redisContext *context)
{
  redisFree(context);
}
