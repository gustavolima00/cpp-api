#pragma once
#include <hiredis/hiredis.h>
#include <cstdlib>
#include <iostream>
#define REDIS_DEFAULT_HOST "localhost"
#define REDIS_DEFAULT_PORT "6379"
using namespace std;

namespace redis_client
{
  redisContext *connect();
  void set_key(redisContext *context, const char *key, const char *value);
  string get_key(redisContext *context, const char *key);
  void disconnect(redisContext *context);

  class RedisClientException : public exception
  {
  public:
    RedisClientException(const char *message);
    const char *what() const noexcept;

  private:
    const char *message;
  };
}
