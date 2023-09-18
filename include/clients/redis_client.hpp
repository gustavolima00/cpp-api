#pragma once
#include <hiredis/hiredis.h>
#include <cstdlib>
#include <iostream>
#include <json_dto/pub.hpp>
#define REDIS_DEFAULT_HOST "localhost"
#define REDIS_DEFAULT_PORT "6379"
using namespace std;

namespace redis_client
{
  class RedisClientException : public exception
  {
  public:
    RedisClientException(const char *message);
    const char *what() const noexcept;

  private:
    const char *message;
  };

  redisContext *connect();
  void set_key(redisContext *context, const char *key, const char *value);
  string get_key(redisContext *context, const char *key);
  void disconnect(redisContext *context);

  template <typename T>
  void push_on_queue(redisContext *context, const char *queue_name, T value)
  {
    string serialized_value = json_dto::to_json(value);
    redisCommand(context, "LPUSH %s %s", queue_name, serialized_value.c_str());
  }

  template <typename T>
  T pop_from_queue(redisContext *context, const char *queue_name)
  {
    redisReply *reply = (redisReply *)redisCommand(context, "RPOP %s", queue_name);
    if (reply->type != REDIS_REPLY_STRING)
    {
      throw RedisClientException("Erro ao executar o comando RPOP");
    }
    T value = json_dto::from_json<T>(reply->str);
    freeReplyObject(reply);

    return value;
  }

  template <typename T>
  vector<T> get_all_from_queue(redisContext *context, const char *queue_name)
  {
    redisReply *reply = (redisReply *)redisCommand(context, "LRANGE %s 0 -1", queue_name);
    if (reply->type != REDIS_REPLY_ARRAY)
    {
      throw RedisClientException("Erro ao executar o comando LRANGE");
    }
    vector<T> values;
    for (size_t i = 0; i < reply->elements; ++i)
    {
      values.push_back(json_dto::from_json<T>(reply->element[i]->str));
    }
    freeReplyObject(reply);
    return values;
  }

  int get_queue_size(redisContext *context, const char *queue_name);
}
