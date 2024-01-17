#ifndef SWALLOW_COMPILER_H
#define SWALLOW_COMPILER_H

#include <fstream>
#include <string>

namespace swallow::compiler
{

  class CompileUnit
  {
  public:
    const std::string FileValue;
    const std::string FilePath;

    inline static CompileUnit * FILE = nullptr;

    explicit CompileUnit(const std::string FilePath)
      : FilePath(FilePath)
      , FileValue(readEntireFile(FilePath))
    {}

  private:
    static std::string readEntireFile(const std::string FilePath)
    {
      constexpr size_t read_size = std::size_t(4096);
      auto stream = std::ifstream(FilePath.data());
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

} // namespace swallow::compiler

#endif /* SWALLOW_COMPILER_H */