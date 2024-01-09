#include "utils.h"

std::string
diagnostics::repeat_string(const std::string & input, size_t amount)
{
  std::string output;
  while (amount--)
    output += input;

  return output;
}
