#ifndef SWALLOW_UTILS_DUMP_H
#define SWALLOW_UTILS_DUMP_H

#include <cstdint>
#include <iostream>

namespace swallow::compiler::utils::dump
{
  void dumpIndent(uint8_t n, std::ostream &to) noexcept;
} // namespace swallow::compiler::utils::dump

#endif