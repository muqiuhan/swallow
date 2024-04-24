#include "diagnostics.h"

#include "panic/panic.hpp"
#include "termcolor.hpp"
#include "utils.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <utility>

namespace swallow::compiler::diagnostics
{

#define DISPLAYED_LINE_PADDING 1

#define COLOR_RED              228, 38, 103
#define COLOR_GREEN            175, 255, 95
#define COLOR_WHITE            220, 238, 235
#define COLOR_GREY             148, 148, 148
#define COLOR_BEACH            125, 199, 164
#define COLOR_LIGHT_GREY       170, 173, 176
#define COLOR_BLUE             0, 116, 217
#define COLOR_ORANGE           255, 133, 27
#define COLOR_YELLOW           255, 220, 0
#define COLOR_AQUA             127, 219, 255

  auto report_type_to_prefix(ReportType type) -> std::string
  {
    switch (type)
      {
#undef ERROR
      case ReportType::ERROR:
        return "E";
      case ReportType::INFO:
        return "I";
      case ReportType::WARNING:
        return "W";
      default:
        utils::Panic("This report type is not implemented yet.");
      }
  }

  auto report_type_to_string(ReportType type) -> std::string
  {
    switch (type)
      {
      case ReportType::ERROR:
        return "Error";
      case ReportType::INFO:
        return "Info";
      case ReportType::WARNING:
        return "Warning";
      default:
        utils::Panic("This report type is not implemented yet.");
      }
#define ERROR 0
  }

  auto color_by_type(std::ostream &stream, const ColorType type) -> std::ostream &
  {
    switch (type)
      {
        case ColorType::DEFAULT: {
          stream << termcolor::color<COLOR_LIGHT_GREY>;
          break;
        }
        case ColorType::RED: {
          stream << termcolor::color<COLOR_RED>;
          break;
        }
        case ColorType::GREEN: {
          stream << termcolor::color<COLOR_GREEN>;
          break;
        }
        case ColorType::BLUE: {
          stream << termcolor::color<COLOR_BLUE>;
          break;
        }
        case ColorType::ORANGE: {
          stream << termcolor::color<COLOR_ORANGE>;
          break;
        }
        case ColorType::YELLOW: {
          stream << termcolor::color<COLOR_YELLOW>;
          break;
        }
        case ColorType::AQUA: {
          stream << termcolor::color<COLOR_AQUA>;
          break;
        }
      default:
        assertm(false, "This color is not implemented yet.");
      }

    return stream;
  }

  auto get_color_by_name(const std::string &name) -> ColorType
  {
    if (name == "RED")
      return ColorType::RED;
    if (name == "GREEN")
      return ColorType::GREEN;
    if (name == "BLUE")
      return ColorType::BLUE;
    if (name == "ORANGE")
      return ColorType::ORANGE;
    if (name == "YELLOW")
      return ColorType::YELLOW;
    if (name == "AQUA")
      return ColorType::AQUA;

    return ColorType::DEFAULT;
  }

  void print_formatted_text(std::ostream &output, const std::string &text)
  {
    output << termcolor::color<COLOR_WHITE>;

    for (auto index = 0U; index < text.length(); index++)
      {
        const auto &current_char = text.at(index);
        if (current_char == '{')
          {
            std::string mode;
            for (index++; index < text.length(); index++)
              {
                const auto &mode_char = text.at(index);
                if (mode_char == '}')
                  break;
                mode += mode_char;
              }

            if (mode == "/")
              output << termcolor::color<COLOR_WHITE>;
            else
              color_by_type(output, get_color_by_name(mode));
          }
        else
          output << current_char;
      }
  }

  auto AscendingLabels::operator()(const Label *first, const Label *second) const -> bool
  {
    auto difference = (int) first->get_span().get_start_index()
                      - (int) second->get_span().get_start_index();
    if (difference == 0)
      return first->get_span().get_end_index() < second->get_span().get_end_index();

    return difference < 0;
  }

  auto DescendingLabels::operator()(const Label *first, const Label *second) const -> bool
  {
    auto difference = (int) first->get_span().get_start_index()
                      - (int) second->get_span().get_start_index();
    if (difference == 0)
      return first->get_span().get_end_index() < second->get_span().get_end_index();

    return difference > 0;
  }

  Span::Span(Details *details, size_t start_index, size_t end_index)
    : start_index_(start_index), details_(details), end_index_(end_index)
  {}

  Span::Span() : start_index_(), details_(), end_index_() {}

  auto Span::relative_to(const Span &span) const -> Span
  {
    return {
      span.get_details(),
      this->start_index_ - span.start_index_,
      this->end_index_ - span.start_index_};
  }

  auto Span::is_inside_span(const Span &span) const -> bool
  {
    return (this->start_index_ <= span.start_index_)
           && (this->end_index_ >= span.end_index_);
  }

  auto Span::get_start_index() const -> size_t { return this->start_index_; }

  auto Span::get_details() const -> Details * { return this->details_; }

  auto Span::get_end_index() const -> size_t { return this->end_index_; }

  void Span::set_end_index(size_t end_index) { this->end_index_ = end_index; }

  auto Span::get_width() const -> size_t { return this->end_index_ - this->start_index_; }

  Label::Label(std::optional<std::string> message, const Span &span, ColorType color_type)
    : message_(std::move(message)), color_(color_type), span_(span)
  {
    this->line_ = this->span_.get_details()->get_label_line(*this);
  }

  auto Label::get_message() const -> const std::optional<std::string> &
  {
    return this->message_;
  }

  auto Label::get_span() const -> const Span & { return this->span_; }

  auto Label::get_color() const -> ColorType { return this->color_; }

  auto Label::get_line() const -> size_t { return this->line_; }

  LabelBuilder::LabelBuilder() = default;

  auto LabelBuilder::with_message(const std::string &message) -> LabelBuilder &
  {
    this->message_ = message;
    return *this;
  }

  auto LabelBuilder::with_color(ColorType color) -> LabelBuilder &
  {
    this->color_ = color;
    return *this;
  }

  auto LabelBuilder::with_span(const Span &span) -> LabelBuilder &
  {
    this->span_ = span;
    return *this;
  }

  auto LabelBuilder::build() -> Label
  {
    assertm(this->span_, "A span is required to build a label.");

    ColorType color = ColorType::DEFAULT;
    if (this->color_)
      color = this->color_.value();

    return {this->message_, this->span_.value(), color};
  }

  Details::Details(std::string source, std::string path)
    : source_(std::move(source)), path_(std::move(path))
  {
    Span *current_span = nullptr;
    for (auto index = 0U; index < this->source_.size(); index++)
      {
        if (current_span == nullptr)
          {
            auto new_span = std::make_shared<Span>(this, index, index);
            current_span = new_span.get();
            this->line_spans_.push_back(new_span);
          }

        const auto &source_char = this->source_.at(index);

        if (source_char == '\n')
          {
            current_span->set_end_index(index);
            current_span = nullptr;
            continue;
          }
      }

    if (current_span != nullptr)
      current_span->set_end_index(this->source_.size() - 1);
  }

  auto Details::get_line_source(const Span &span) const -> std::string
  {
    auto result = this->source_.substr(span.get_start_index(), span.get_width());
    result = std::regex_replace(result, std::regex("\t"), " ");
    return result;
  }

  auto Details::get_label_line(const Label &label) const -> size_t
  {
    for (auto index = 0U; index < this->line_spans_.size(); index++)
      {
        const auto &span = this->line_spans_.at(index);
        if (span->is_inside_span(label.get_span()))
          return index;
      }

    utils::Panic("Couldn't find the associated line for this span.");
  }

  auto Details::get_line_spans() const -> const std::vector<std::shared_ptr<Span>> &
  {
    return this->line_spans_;
  }

  auto Details::get_source() const -> const std::string & { return this->source_; }

  auto Details::get_path() const -> const std::string & { return this->path_; }

  LabelGroup::LabelGroup(Details *general_details_, std::vector<const Label *> labels)
    : first_label_()
    , last_label_()
    , details_(general_details_)
    , labels_(std::move(labels))
  {
    assertm(
      !this->labels_.empty(), "Couldn't find the last labels as there are no labels.");

    auto ascending_labels(this->labels_);
    std::sort(ascending_labels.begin(), ascending_labels.end(), AscendingLabels());

    this->first_label_ = ascending_labels.front();
    this->last_label_ = ascending_labels.back();
  }

  void LabelGroup::print(std::ostream &output, const std::string &spaces_prefix) const
  {
    auto first_line = this->first_label_->get_line();
    auto last_line = this->last_label_->get_line();

    auto beginning_line = 0U;
    if (first_line >= DISPLAYED_LINE_PADDING)
      beginning_line = first_line - DISPLAYED_LINE_PADDING;

    auto ending_line = last_line + DISPLAYED_LINE_PADDING;
    if (ending_line >= this->details_->get_line_spans().size())
      ending_line = last_line;

    for (auto line_index = beginning_line; line_index <= ending_line; line_index++)
      {
        const auto &line_span = this->details_->get_line_spans()[line_index];

        auto        line_number = line_index + 1;
        output << "  "
               << COLOR_RGB(
                    std::setw(spaces_prefix.length() - 3)
                      << std::setfill(' ') << line_number << " |  ",
                    COLOR_GREY);

        auto labels = this->find_labels_in_line(line_index);
        this->print_colored_source_line(output, *line_span, labels);

        if (labels.empty())
          continue;

        auto label_levels = find_label_levels(labels);
        for (auto index = 0U; index < label_levels.size(); index++)
          print_labels_level(label_levels, index, *line_span, output, spaces_prefix);
      }
  }

  void LabelGroup::print_labels_level(
    const std::vector<Labels> &level_labels,
    size_t                     current_level,
    const Span                &line_span,
    std::ostream              &output,
    const std::string         &spaces_prefix)
  {
    std::map<size_t, const Label *> next_label_startings;
    std::map<size_t, const Label *> next_label_endings;
    for (auto index = current_level + 1; index < level_labels.size(); index++)
      {
        const auto &labels = level_labels.at(index);
        for (const auto &label : labels)
          {
            const auto relative_span = label->get_span().relative_to(line_span);
            next_label_startings[relative_span.get_start_index()] = label;
            next_label_endings[relative_span.get_end_index()] = label;
          }
      }

    const auto                     &current_labels = level_labels.at(current_level);

    std::map<size_t, const Label *> current_label_startings;
    for (const auto &label : current_labels)
      {
        const auto relative_span = label->get_span().relative_to(line_span);
        current_label_startings[relative_span.get_start_index()] = label;
      }

    output << spaces_prefix << COLOR_RGB("o  ", COLOR_GREY);

    const Label *last_label = nullptr;
    size_t       last_end_index = 0U;
    for (auto index = 0U; index < line_span.get_width(); index++)
      {
        if (next_label_endings.contains(index))
          {
            auto &next_label = next_label_endings.at(index);
            COLOR_BY_TYPE(next_label->get_color(), "|");
            continue;
          }
        if (next_label_startings.contains(index))
          {
            auto &next_label = next_label_startings.at(index);
            COLOR_BY_TYPE(next_label->get_color(), "|");
            continue;
          }

        if (!current_label_startings.contains(index))
          {
            if (index == last_end_index && index != 0)
              COLOR_BY_TYPE(last_label->get_color(), "^");
            else if (index < last_end_index)
              COLOR_BY_TYPE(last_label->get_color(), "^");
            else
              output << " ";

            continue;
          }

        const auto *label = current_label_startings.at(index);
        auto        relative_span = label->get_span().relative_to(line_span);

        if (last_end_index >= index && index != 0)
          if (label->get_message())
            COLOR_BY_TYPE(label->get_color(), "=");
          else
            COLOR_BY_TYPE(label->get_color(), "+");
        else if (relative_span.get_end_index() > index)
          if (label->get_message())
            COLOR_BY_TYPE(label->get_color(), "^");
          else
            COLOR_BY_TYPE(label->get_color(), "+");
        else if (label->get_message())
          COLOR_BY_TYPE(label->get_color(), "^");
        else
          COLOR_BY_TYPE(label->get_color(), "^");

        last_end_index = relative_span.get_end_index();
        last_label = label;
      }

    output << "\n";

    for (const auto &label : current_labels)
      {
        if (!label->get_message())
          continue;

        output << spaces_prefix << COLOR_RGB("o  ", COLOR_GREY);

        const auto relative_span = label->get_span().relative_to(line_span);
        for (auto index = 0U; index < relative_span.get_start_index(); index++)
          {
            if (next_label_endings.contains(index))
              {
                auto &next_label = next_label_endings.at(index);
                COLOR_BY_TYPE(next_label->get_color(), "|");
                continue;
              }

            if (next_label_startings.contains(index))
              {
                auto &next_label = next_label_startings.at(index);
                COLOR_BY_TYPE(next_label->get_color(), "|");
                continue;
              }

            if (current_label_startings.contains(index))
              {
                auto &next_label = current_label_startings.at(index);
                COLOR_BY_TYPE(next_label->get_color(), "|");
                continue;
              }

            output << " ";
          }

        COLOR_BY_TYPE(label->get_color(), ":= ");
        print_formatted_text(output, label->get_message().value());
        output << "\n";
      }
  }

  void LabelGroup::print_colored_source_line(
    std::ostream &output, const Span &label_span, const Labels &labels) const
  {
    const auto                      source = this->details_->get_line_source(label_span);

    std::map<size_t, const Label *> mapped_labels;
    for (const auto &label : labels)
      {
        const auto &line_span = this->details_->get_line_spans()[label->get_line()];
        auto        relative_span = label->get_span().relative_to(*line_span);
        mapped_labels[relative_span.get_start_index()] = label;
      }

    output << termcolor::color<COLOR_LIGHT_GREY>;
    for (auto char_index = 0U; char_index < source.length(); char_index++)
      {
        const auto &current_char = source.at(char_index);
        if (!mapped_labels.contains(char_index))
          {
            output << current_char;
            continue;
          }

        const auto &label = mapped_labels.at(char_index);

        color_by_type(output, label->get_color());
        output << current_char;

        auto starting_index = char_index;
        for (char_index++;
             char_index < starting_index + label->get_span().get_width() + 1;
             char_index++)
          {
            if (mapped_labels.contains(char_index))
              break;

            // TODO: Support multi-line
            if (char_index >= source.length())
              break;

            output << source.at(char_index);
          }
        char_index--;

        output << termcolor::color<COLOR_LIGHT_GREY>;
      }

    output << termcolor::reset << "\n";
  }

  auto LabelGroup::find_label_levels(const Labels &labels) -> std::vector<Labels>
  {
    auto descending_labels(labels);
    std::sort(descending_labels.begin(), descending_labels.end(), DescendingLabels());

    std::vector<Labels> level_labels;

    auto                current_labels = descending_labels;
    while (true)
      {
        auto overlapping_labels = find_remove_overlapping_labels(current_labels);
        level_labels.push_back(current_labels);

        if (overlapping_labels.empty())
          break;

        current_labels = overlapping_labels;
      }

    return level_labels;
  }

  auto LabelGroup::find_remove_overlapping_labels(Labels &labels) -> Labels
  {
    if (labels.empty())
      return {};

    Labels      overlapping_labels;

    const auto *current_label = labels.front();
    for (auto iterator = labels.begin() + 1; iterator < labels.end(); iterator++)
      {
        const auto *next_label = *iterator;

        if (
          next_label->get_span().get_end_index()
          < current_label->get_span().get_start_index())
          current_label = next_label;
        else
          {
            overlapping_labels.push_back(next_label);
            iterator = labels.erase(iterator) - 1;
          }
      }

    return overlapping_labels;
  }

  auto LabelGroup::find_labels_in_line(size_t line_index) const -> Labels
  {
    Labels      result;

    const auto &line_span = this->details_->get_line_spans().at(line_index);
    for (const auto &label : this->labels_)
      if (line_span->is_inside_span(label->get_span()))
        result.push_back(label);

    return result;
  }

  auto LabelGroup::get_labels() const -> const Labels & { return this->labels_; }

  auto LabelGroup::get_first_label() const -> const Label * { return this->first_label_; }

  auto LabelGroup::get_last_label() const -> const Label * { return this->last_label_; }

  auto LabelGroup::get_details() const -> Details * { return this->details_; }

  FileGroup::FileGroup(Details *details, Labels labels) : details_(details)
  {
    assertm(
      !labels.empty(),
      "Cannot find label current_labels if there are no "
      "labels_collection.");

    std::vector<Labels> labels_collection;
    auto               *current_labels = &labels_collection.emplace_back();

    auto                ascending_labels(labels);
    std::sort(ascending_labels.begin(), ascending_labels.end(), AscendingLabels());

    auto last_line = labels.front()->get_line();
    for (const auto &label : ascending_labels)
      {
        auto label_line = label->get_line();
        auto line_difference = (int32_t) label_line - (int32_t) last_line;

        if (line_difference > DISPLAYED_LINE_PADDING)
          current_labels = &labels_collection.emplace_back();

        current_labels->push_back(label);
        last_line = label_line;
      }

    for (const auto &collected_labels : labels_collection)
      this->label_groups_.emplace_back(details, collected_labels);
  }

  void FileGroup::print(std::ostream &output, const std::string &spaces_prefix) const
  {
    output << COLOR_RGB("-[", COLOR_GREY)
           << COLOR_RGB(this->details_->get_path(), COLOR_WHITE)
           << COLOR_RGB("]", COLOR_GREY);
    output << "\n";

    output << spaces_prefix << COLOR_RGB("o", COLOR_GREY);
    output << "\n";

    for (auto index = 0U; index < this->label_groups_.size(); index++)
      {
        const auto &labels_group = this->label_groups_.at(index);
        labels_group.print(output, spaces_prefix);

        if (index != this->label_groups_.size() - 1)
          {
            output << spaces_prefix << COLOR_RGB("⋮", COLOR_GREY);
            output << "\n";
          }
      }
  }

  auto FileGroup::get_biggest_displayed_number() const -> size_t
  {
    auto biggest_number = 0U;
    for (const auto &labels_group : this->label_groups_)
      {
        const auto *last_label = labels_group.get_last_label();
        auto        line_number = last_label->get_line();

        if (biggest_number < line_number)
          biggest_number = line_number;
      }

    // Add 1 to get a number starting at 1. Add the line padding to get the
    // last line displayed by the report.
    biggest_number += 1 + DISPLAYED_LINE_PADDING;

    return biggest_number;
  }

  auto FileGroup::get_label_groups() const -> const std::vector<LabelGroup> &
  {
    return this->label_groups_;
  }

  auto FileGroup::get_details() const -> Details * { return this->details_; }

  Report::Report(
    ReportType                 type,
    std::string                message,
    size_t                     code,
    std::vector<Label>         labels,
    std::optional<std::string> note)
    : note_(std::move(note))
    , labels_(std::move(labels))
    , message_(std::move(message))
    , type_(type)
    , code_(code)
  {}

  void Report::print(std::ostream &output) const
  {
    output << COLOR_RGB(
      "[" << report_type_to_prefix(this->type_) << std::setw(3) << std::setfill('0')
          << this->code_ << "] " << report_type_to_string(this->type_) << ":",
      COLOR_RED)
           << " " << COLOR_RGB(this->message_, COLOR_WHITE);
    output << "\n";

    auto file_groups = this->find_file_groups();

    auto biggest_number = 0U;
    for (const auto &file_group : file_groups)
      {
        auto file_biggest_number = file_group.get_biggest_displayed_number();

        if (file_biggest_number > biggest_number)
          biggest_number = file_biggest_number;
      }

    auto biggest_number_width = std::to_string(biggest_number).length();
    auto spaces_prefix = std::string(biggest_number_width + 3, ' ');

    output << spaces_prefix << COLOR_RGB("/", COLOR_GREY);
    for (auto index = 0U; index < file_groups.size(); index++)
      {
        const auto &file_group = file_groups.at(index);
        file_group.print(output, spaces_prefix);

        if (index != file_groups.size() - 1)
          {
            output << spaces_prefix << COLOR_RGB("o", COLOR_GREY);
            output << "\n";

            output << spaces_prefix << COLOR_RGB("^", COLOR_GREY);
          }
      }

    output << spaces_prefix << COLOR_RGB("o", COLOR_GREY);
    output << "\n";

    if (this->note_)
      {
        output << spaces_prefix << COLOR_RGB("o-- ", COLOR_GREY)
               << COLOR_RGB("Note: ", COLOR_BEACH);
        print_formatted_text(output, this->note_.value());
        output << "\n";
      }

    auto dashes_prefix = biggest_number_width + 3;
    output << COLOR_RGB(repeat_string("-", dashes_prefix), COLOR_GREY)
           << COLOR_RGB("/", COLOR_GREY);
    output << "\n";
  }

  auto Report::find_file_groups() const -> std::vector<FileGroup>
  {
    std::map<Details *, Labels> group_mappings;
    for (const auto &label : this->labels_)
      {
        auto *details = label.get_span().get_details();
        auto  result = group_mappings.find(details);

        if (result == group_mappings.end())
          group_mappings[details] = {&label};
        else
          result->second.push_back(&label);
      }

    std::vector<FileGroup> file_groups;
    file_groups.reserve(group_mappings.size());

    for (const auto &[details, labels] : group_mappings)
      file_groups.emplace_back(details, labels);

    return file_groups;
  }

  auto Report::get_note() const -> const std::optional<std::string> &
  {
    return this->note_;
  }

  auto Report::get_labels() const -> const std::vector<Label> & { return this->labels_; }

  auto Report::get_message() const -> const std::string & { return this->message_; }

  auto Report::get_type() const -> ReportType { return this->type_; }

  auto Report::get_code() const -> size_t { return this->code_; }

  ReportBuilder::ReportBuilder() = default;

  auto ReportBuilder::with_message(const std::string &message) -> ReportBuilder &
  {
    this->message_ = message;
    return *this;
  }

  auto ReportBuilder::with_note(const std::string &note) -> ReportBuilder &
  {
    this->note_ = note;
    return *this;
  }

  auto ReportBuilder::add_label(const Label &label) -> ReportBuilder &
  {
    this->labels_.push_back(label);
    return *this;
  }

  auto ReportBuilder::with_type(ReportType type) -> ReportBuilder &
  {
    this->type_ = type;
    return *this;
  }

  auto ReportBuilder::with_code(size_t code) -> ReportBuilder &
  {
    this->code_ = code;
    return *this;
  }

  auto ReportBuilder::build() -> Report
  {
    assertm(this->type_, "A type is required to build a report.");
    assertm(this->message_, "A message is required to build a report.");
    assertm(this->code_, "A message is required to build a report.");
    return {
      this->type_.value(),
      this->message_.value(),
      this->code_.value(),
      this->labels_,
      this->note_};
  }
} // namespace swallow::compiler::diagnostics
