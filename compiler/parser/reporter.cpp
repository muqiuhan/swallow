#include "reporter.h"
#include "panic/panic.hpp"

namespace swallow::compiler::parser
{

  void ParserReporter::normal(const yy::parser::location_type & loc,
                              const std::string && msg,
                              const std::string && labelMsg,
                              const std::string && note)
  {

    diagnostics::Report report =
      diagnostics::ReportBuilder()
        .with_type(swallow::diagnostics::ReportType::ERROR)
        .with_message(msg)
        .with_code(0x0001)
        .add_label(
          swallow::diagnostics::LabelBuilder()
            .with_message(labelMsg)
            .with_span({ &Details, loc.begin.column, loc.end.column - 1 })
            .with_color(swallow::diagnostics::ColorType::RED)
            .build())
        .with_note(note)
        .build();

    report.print(std::cout);

    utils::panic("Parse failed at ({}:{}) ({}:{})",
                 loc.begin.line,
                 loc.begin.column,
                 loc.end.line,
                 loc.end.column);
  }

} // namespace swallow::compiler::parser