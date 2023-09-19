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
  void delete_key(redisContext *context, const char *key);
  void disconnect(redisContext *context);

  void push_on_queue(redisContext *context, const char *queue_name, const char *value);
  string pop_from_queue(redisContext *context, const char *queue_name);
  vector<string> get_all_from_queue(redisContext *context, const char *queue_name);
  void delete_queue(redisContext *context, const char *queue_name);

  int get_queue_size(redisContext *context, const char *queue_name);
}
