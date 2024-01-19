#include "utils.h"

namespace swallow::compiler::diagnostics
{
  auto repeat_string(const std::string &input, size_t amount) -> std::string
  {
    std::string output;
    while ((amount--) != 0U)
      {
        output += input;
      }

    return output;
  }
} // namespace swallow::compiler::diagnostics