#ifndef SWALLOW_COMPILER_DIAGNOSTICS_DIAGNOSTICS_H
#define SWALLOW_COMPILER_DIAGNOSTICS_DIAGNOSTICS_H

#include <cassert>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#define COLOR_BY_TYPE(type, text)                                              \
  color_by_type(std::cout, type) << text << termcolor::reset
#define COLOR_RGB(text, rgb) termcolor::color<rgb> << text << termcolor::reset

#define assertm(exp, msg) assert(((void) msg, exp))

namespace swallow::compiler::diagnostics
{

  enum class ReportType
  {
    ERROR,
    INFO,
    WARNING,
  };

  auto report_type_to_prefix(ReportType type) -> std::string;

  auto report_type_to_string(ReportType type) -> std::string;

  enum class ColorType
  {
    DEFAULT,
    RED,
    GREEN,
    BLUE,
    ORANGE,
    YELLOW,
    AQUA
  };

  auto color_by_type(std::ostream &stream, ColorType type) -> std::ostream &;

  auto get_color_by_name(const std::string &name) -> ColorType;

  auto print_formatted_text(std::ostream &output,
                            const std::string &text) -> void;

  class Label;

  class Details;

  class Span
  {
  public:
    Span(Details *details, size_t start_index, size_t end_index);

    Span();

    [[nodiscard]] auto relative_to(const Span &span) const -> Span;

    [[nodiscard]] auto is_inside_span(const Span &span) const -> bool;

    [[nodiscard]] auto get_start_index() const -> size_t;

    [[nodiscard]] auto get_details() const -> Details *;

    [[nodiscard]] auto get_end_index() const -> size_t;

    void set_end_index(size_t end_index);

    [[nodiscard]] auto get_width() const -> size_t;

  private:
    size_t start_index_;
    Details *details_;
    size_t end_index_;
  };

  class Label
  {
  public:
    Label(std::optional<std::string> message, const Span &span,
          ColorType color_type);

    [[nodiscard]] auto
      get_message() const -> const std::optional<std::string> &;

    [[nodiscard]] auto get_span() const -> const Span &;

    [[nodiscard]] auto get_color() const -> ColorType;

    [[nodiscard]] auto get_line() const -> size_t;

  private:
    std::optional<std::string> message_;
    ColorType color_;
    size_t line_;
    Span span_;
  };

  class LabelBuilder
  {
  public:
    LabelBuilder();

    auto with_message(const std::string &message) -> LabelBuilder &;

    auto with_color(ColorType color) -> LabelBuilder &;

    auto with_span(const Span &span) -> LabelBuilder &;

    auto build() -> Label;

  private:
    std::optional<std::string> message_;
    std::optional<ColorType> color_;
    std::optional<Span> span_;
  };

  struct AscendingLabels
  {
    auto operator()(const Label *first, const Label *second) const -> bool;
  };

  struct DescendingLabels
  {
    auto operator()(const Label *first, const Label *second) const -> bool;
  };

  class Details
  {
  public:
    Details(std::string source, std::string path);

    [[nodiscard]] auto get_line_source(const Span &span) const -> std::string;

    [[nodiscard]] auto get_label_line(const Label &label) const -> size_t;

    [[nodiscard]] auto
      get_line_spans() const -> const std::vector<std::shared_ptr<Span>> &;

    [[nodiscard]] auto get_source() const -> const std::string &;

    [[nodiscard]] auto get_path() const -> const std::string &;

  private:
    std::vector<std::shared_ptr<Span>> line_spans_;
    std::string source_;
    std::string path_;
  };

  using Labels = std::vector<const Label *>;

  class LabelGroup
  {
  public:
    LabelGroup(Details *general_details, Labels labels);

    auto print(std::ostream &output,
               const std::string &spaces_prefix) const -> void;

    static auto print_labels_level(const std::vector<Labels> &level_labels,
                                   size_t current_level, const Span &line_span,
                                   std::ostream &output,
                                   const std::string &spaces_prefix) -> void;

    auto print_colored_source_line(std::ostream &output, const Span &label_span,
                                   const Labels &labels) const -> void;

    [[nodiscard]] static auto
      find_label_levels(const Labels &labels) -> std::vector<Labels>;

    [[nodiscard]] static auto
      find_remove_overlapping_labels(Labels &labels) -> Labels;

    [[nodiscard]] auto find_labels_in_line(size_t line_index) const -> Labels;

    [[nodiscard]] auto get_first_label() const -> const Label *;

    [[nodiscard]] auto get_labels() const -> const Labels &;

    [[nodiscard]] auto get_details() const -> Details *;

    [[nodiscard]] auto get_last_label() const -> const Label *;

  private:
    const Label *first_label_;
    const Label *last_label_;
    Details *details_;
    Labels labels_;
  };

  class FileGroup
  {
  public:
    FileGroup(Details *details, Labels labels);

    auto print(std::ostream &output,
               const std::string &spaces_prefix) const -> void;

    [[nodiscard]] auto get_biggest_displayed_number() const -> size_t;

    [[nodiscard]] auto
      get_label_groups() const -> const std::vector<LabelGroup> &;

    [[nodiscard]] auto get_details() const -> Details *;

  private:
    std::vector<LabelGroup> label_groups_;
    Details *details_;
  };

  class Report
  {
  public:
    Report(ReportType type, std::string message, size_t code,
           std::vector<Label> labels, std::optional<std::string> note);

    auto print(std::ostream &output) const -> void;

    [[nodiscard]] auto find_file_groups() const -> std::vector<FileGroup>;

    [[nodiscard]] auto get_note() const -> const std::optional<std::string> &;

    [[nodiscard]] auto get_labels() const -> const std::vector<Label> &;

    [[nodiscard]] auto get_message() const -> const std::string &;

    [[nodiscard]] auto get_type() const -> ReportType;

    [[nodiscard]] auto get_code() const -> size_t;

  private:
    std::optional<std::string> note_;
    std::vector<Label> labels_;
    std::string message_;
    ReportType type_;
    size_t code_;
  };

  class ReportBuilder
  {
  public:
    ReportBuilder();

    auto with_message(const std::string &message) -> ReportBuilder &;

    auto with_note(const std::string &note) -> ReportBuilder &;

    auto add_label(const Label &label) -> ReportBuilder &;

    auto with_type(ReportType type) -> ReportBuilder &;

    auto with_code(size_t code) -> ReportBuilder &;

    auto build() -> Report;

  private:
    std::optional<std::string> message_;
    std::optional<std::string> note_;
    std::optional<ReportType> type_;
    std::optional<size_t> code_;
    std::vector<Label> labels_;
  };

} // namespace swallow::compiler::diagnostics

#endif // SWALLOW_COMPILER_DIAGNOSTICS_DIAGNOSTICS_H
