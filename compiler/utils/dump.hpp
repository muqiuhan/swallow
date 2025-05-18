#ifndef SWALLOW_COMPILER_UTILS_DUMP_H
#define SWALLOW_COMPILER_UTILS_DUMP_H

#include <cstdint>
#include <iostream>

namespace swallow::compiler::utils::dump
{
  static void DumpIndent(uint8_t n, std::ostream &to)
  {
    while ((n--) != 0U)
      to << "  ";
  }
} // namespace swallow::compiler::utils::dump

#endif