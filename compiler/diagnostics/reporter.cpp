#include "reporter.h"
#include "panic/panic.hpp"

using namespace swallow::utils;

namespace swallow::compiler::diagnostics
{

  [[noreturn]] Err<Void> Reporter::normal(const yy::parser::location_type & loc,
                                          const std::string && msg,
                                          const std::string && labelMsg,
                                          const std::string && note,
                                          const std::uint32_t && code)
  {

    ReportBuilder()
      .with_type(ReportType::ERROR)
      .with_message(msg)
      .with_code(code)
      .add_label(LabelBuilder()
                   .with_message(labelMsg)
                   .with_span({ &Detail, loc.begin.column, loc.end.column - 1 })
                   .with_color(ColorType::RED)
                   .build())
      .with_note(note)
      .build()
      .print(std::cout);

    utils::panic("Parse failed at ({}:{}) ({}:{})",
                 loc.begin.line,
                 loc.begin.column,
                 loc.end.line,
                 loc.end.column);

    return { Void() };
  }

} // namespace swallow::compiler::diagnostics