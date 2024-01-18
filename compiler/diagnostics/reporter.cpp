#include "reporter.h"
#include "panic/panic.hpp"
#include <cstdlib>

using namespace swallow::utils;

namespace swallow::compiler::diagnostics
{

  [[noreturn]] Err<Void> Reporter::normal(const yy::parser::location_type & loc,
                                          const std::string && msg,
                                          const std::string && labelMsg,
                                          const std::string && note,
                                          const std::uint32_t & code)
  {

    ReportBuilder()
      .with_type(ReportType::ERROR)
      .with_message(msg)
      .with_code(code)
      .add_label(
        LabelBuilder()
          .with_message(labelMsg)
          .with_span({&Detail, static_cast<size_t>(loc.begin.column - 1),
                      static_cast<size_t>(loc.end.column - 1)})
          .with_color(ColorType::RED)
          .build())
      .with_note(note)
      .build()
      .print(std::cout);

    exit(EXIT_FAILURE);
    return {Void()};
  }

} // namespace swallow::compiler::diagnostics