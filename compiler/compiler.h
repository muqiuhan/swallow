// Copyright (c) 2023 Muqiu Han
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//     notice,
//       this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Swallow nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef SWALLOW_COMPILER_COMPILER_H
#define SWALLOW_COMPILER_COMPILER_H

#include <fstream>
#include <optional>
#include <string>

namespace swallow::compiler
{
  class CompileUnit
  {
  public:
    const std::string FileValue;
    const std::string FilePath;

    inline static CompileUnit *FILE = nullptr;

    explicit CompileUnit(const std::string &FilePath)
      : FilePath(FilePath), FileValue(ReadEntireFile(FilePath))
    {}

  private:
    static auto ReadEntireFile(const std::string &FilePath) -> std::string
    {
      constexpr auto read_size = std::size_t(4096);
      auto           stream = std::ifstream(FilePath.data());
      stream.exceptions(std::ios_base::badbit);

      if (!stream)
        throw std::ios_base::failure("file does not exist");

      auto out = std::string();
      auto buf = std::string(read_size, '\0');

      while (stream.read(&buf[0], read_size))
        out.append(buf, 0, stream.gcount());

      out.append(buf, 0, stream.gcount());
      return out;
    }
  };

  class CompilerOptions
  {
  public:
    std::string         file;
    std::optional<bool> dump_ast;
    std::optional<bool> dump_types;
    std::optional<bool> dump_gmachine_ir;
    std::optional<bool> verbose = false;

  public:
    inline static std::string HELP =
      "USAGE: swa compile [FLAGS] [OPTIONS] file \n"
      "\n"
      "FLAGS:\n"
      "    -v, --verbose\tThe swallow source file path\n"
      "    -d, --dump_ast\tDump the ast\n"
      "    -d, --dump_types\tDump the definition types\n"
      "    -d, --dump_gmachine_ir\tDump the G-Machine instructions\n"
      "\n"
      "OPTIONS:\n"
      "    -h, --help <help>\n"
      "    --version <version>\n"
      "\n"
      "ARGS:\n"
      "    file";
    inline static std::string VERSION = "0.0.1";
  };

  auto Compiler(const CompilerOptions &options) noexcept -> int;
} // namespace swallow::compiler

#endif // SWALLOW_COMPILER_COMPILER_H