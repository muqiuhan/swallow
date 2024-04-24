#include "dump.h"

namespace swallow::compiler::utils::dump
{
  void dumpIndent(uint8_t n, std::ostream &to) noexcept
  {
    while ((n--) != 0U)
      to << "  ";
  }
} // namespace swallow::compiler::utils::dump