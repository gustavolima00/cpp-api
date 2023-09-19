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

void redis_client::delete_key(redisContext *context, const char *key)
{
  redisReply *reply = (redisReply *)redisCommand(context, "DEL %s", key);
  if (reply->type != REDIS_REPLY_INTEGER)
  {
    throw RedisClientException("Erro ao executar o comando DEL");
  }
  freeReplyObject(reply);
}

void redis_client::disconnect(redisContext *context)
{
  redisFree(context);
}

int redis_client::get_queue_size(redisContext *context, const char *queue_name)
{
  redisReply *reply = (redisReply *)redisCommand(context, "LLEN %s", queue_name);
  if (reply->type != REDIS_REPLY_INTEGER)
  {
    throw RedisClientException("Erro ao executar o comando LLEN");
  }
  int size = reply->integer;
  freeReplyObject(reply);
  return size;
}

void redis_client::push_on_queue(redisContext *context, const char *queue_name, const char *value)
{
  redisCommand(context, "LPUSH %s %s", queue_name, value);
}

string redis_client::pop_from_queue(redisContext *context, const char *queue_name)
{
  redisReply *reply = (redisReply *)redisCommand(context, "RPOP %s", queue_name);
  if (reply->type != REDIS_REPLY_STRING)
  {
    throw RedisClientException("Erro ao executar o comando RPOP");
  }
  string value(reply->str);
  freeReplyObject(reply);

  return value;
}

vector<string> redis_client::get_all_from_queue(redisContext *context, const char *queue_name)
{
  redisReply *reply = (redisReply *)redisCommand(context, "LRANGE %s 0 -1", queue_name);
  if (reply == NULL)
  {
    return vector<string>();
  }
  if (reply->type != REDIS_REPLY_ARRAY)
  {
    freeReplyObject(reply);
    throw RedisClientException("Erro ao executar o comando LRANGE");
  }
  vector<string> values;
  for (size_t i = 0; i < reply->elements; i++)
  {
    values.push_back(reply->element[i]->str);
  }
  freeReplyObject(reply);
  return values;
}

void redis_client::delete_queue(redisContext *context, const char *queue_name)
{
  redisCommand(context, "DEL %s", queue_name);
}
