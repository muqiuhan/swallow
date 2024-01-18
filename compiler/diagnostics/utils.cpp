#include "utils.h"

namespace swallow::compiler::diagnostics
{
  std::string repeat_string(const std::string & input, size_t amount)
  {
    std::string output;
    while (amount--)
      output += input;

    return output;
  }
} // namespace swallow::compiler::diagnostics