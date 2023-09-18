#include "requests/base_request_handler.hpp"

string base_request_handler::url_decode(const string &str)
{
  ostringstream unescaped;
  for (size_t i = 0; i < str.size(); ++i)
  {
    if (str[i] == '%' && i + 2 < str.size())
    {
      int value;
      istringstream is(str.substr(i + 1, 2));
      if (is >> hex >> value)
      {
        unescaped << static_cast<char>(value);
        i += 2;
      }
      else
      {
        return "";
      }
    }
    else if (str[i] == '+')
    {
      unescaped << ' ';
    }
    else
    {
      unescaped << str[i];
    }
  }
  return unescaped.str();
}