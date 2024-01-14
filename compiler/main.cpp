#include "diagnostics/diagnostics.h"
#include "parser/parser.h"
#include <iostream>

using namespace swallow;

auto main(int argc, char ** argv) -> int
{
  auto details = diagnostics::Details{
    "This is a test text\nMaybe you could have guessed that.",
    "some/weird/path"
  };

  auto report =
    diagnostics::ReportBuilder()
      .with_type(diagnostics::ReportType::ERROR)
      .with_message("Displaying a brief summary of what happend.")
      .with_code(1337)
      .add_label(diagnostics::LabelBuilder()
                   .with_message("Giving some {RED}tips{/} or "
                                 "{ORANGE}extra details{/} about "
                                 "what is wrong here.")
                   .with_span({ &details, 10, 13 })
                   .with_color(diagnostics::ColorType::RED)
                   .build())
      .with_note("Can be used to give a hint about what you could do better.")
      .build();

  // report.print(std::cout);

  swallow::parser::parse();
  return 0;
}
