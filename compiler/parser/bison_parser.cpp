// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

// First part of user prologue.
#line 7 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"

#include <string>
#include <iostream>
#include "bison_parser.hpp"
#include "diagnostics/reporter.hpp"
#include "utils/binop.hpp"

std::vector<Definition::Ptr>   Program;
extern yy::parser::symbol_type yylex();

#line 53 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"

#include "bison_parser.hpp"

#ifndef YY_
#if defined YYENABLE_NLS && YYENABLE_NLS
#if ENABLE_NLS
#include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#define YY_(msgid) dgettext("bison-runtime", msgid)
#endif
#endif
#ifndef YY_
#define YY_(msgid) msgid
#endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
#if defined __GNUC__ && !defined __EXCEPTIONS
#define YY_EXCEPTIONS 0
#else
#define YY_EXCEPTIONS 1
#endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(Current, Rhs, N)                                        \
  do                                                                           \
    if (N)                                                                     \
      {                                                                        \
        (Current).begin = YYRHSLOC(Rhs, 1).begin;                              \
        (Current).end = YYRHSLOC(Rhs, N).end;                                  \
      }                                                                        \
    else                                                                       \
      {                                                                        \
        (Current).begin = (Current).end = YYRHSLOC(Rhs, 0).end;                \
      }                                                                        \
  while (false)
#endif

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
#define YYCDEBUG                                                               \
  if (yydebug_)                                                                \
  (*yycdebug_)

#define YY_SYMBOL_PRINT(Title, Symbol)                                         \
  do                                                                           \
    {                                                                          \
      if (yydebug_)                                                            \
        {                                                                      \
          *yycdebug_ << Title << ' ';                                          \
          yy_print_(*yycdebug_, Symbol);                                       \
          *yycdebug_ << '\n';                                                  \
        }                                                                      \
  } while (false)

#define YY_REDUCE_PRINT(Rule)                                                  \
  do                                                                           \
    {                                                                          \
      if (yydebug_)                                                            \
        yy_reduce_print_(Rule);                                                \
  } while (false)

#define YY_STACK_PRINT()                                                       \
  do                                                                           \
    {                                                                          \
      if (yydebug_)                                                            \
        yy_stack_print_();                                                     \
  } while (false)

#else // !YYDEBUG

#define YYCDEBUG                                                               \
  if (false)                                                                   \
  std::cerr
#define YY_SYMBOL_PRINT(Title, Symbol) YY_USE(Symbol)
#define YY_REDUCE_PRINT(Rule)          static_cast<void>(0)
#define YY_STACK_PRINT()               static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok        (yyerrstatus_ = 0)
#define yyclearin      (yyla.clear())

#define YYACCEPT       goto yyacceptlab
#define YYABORT        goto yyabortlab
#define YYERROR        goto yyerrorlab
#define YYRECOVERING() (!!yyerrstatus_)

namespace yy
{
#line 150 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"

  /// Build a parser object.
  parser::parser()
#if YYDEBUG
    : yydebug_(false), yycdebug_(&std::cerr)
#else

#endif
  {}

  parser::~parser() {}

  parser::syntax_error::~syntax_error() YY_NOEXCEPT YY_NOTHROW {}

  /*---------.
  | symbol.  |
  `---------*/

  // by_state.
  parser::by_state::by_state() YY_NOEXCEPT : state(empty_state) {}

  parser::by_state::by_state(const by_state& that) YY_NOEXCEPT
    : state(that.state)
  {}

  void parser::by_state::clear() YY_NOEXCEPT { state = empty_state; }

  void parser::by_state::move(by_state& that)
  {
    state = that.state;
    that.clear();
  }

  parser::by_state::by_state(state_type s) YY_NOEXCEPT : state(s) {}

  parser::symbol_kind_type parser::by_state::kind() const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST(symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type() {}

  parser::stack_symbol_type::stack_symbol_type(YY_RVREF(stack_symbol_type) that)
    : super_type(YY_MOVE(that.state), YY_MOVE(that.location))
  {
    switch (that.kind())
      {
      case symbol_kind::S_Add:             // Add
      case symbol_kind::S_Mul:             // Mul
      case symbol_kind::S_Application:     // Application
      case symbol_kind::S_ApplicationBase: // ApplicationBase
      case symbol_kind::S_Match:           // Match
        value.YY_MOVE_OR_COPY<AST::Ptr>(YY_MOVE(that.value));
        break;

      case symbol_kind::S_Branch: // Branch
        value.YY_MOVE_OR_COPY<Branch::Ptr>(YY_MOVE(that.value));
        break;

      case symbol_kind::S_Constructor: // Constructor
        value.YY_MOVE_OR_COPY<Constructor::Ptr>(YY_MOVE(that.value));
        break;

      case symbol_kind::S_Definition: // Definition
      case symbol_kind::S_Fn:         // Fn
      case symbol_kind::S_Data:       // Data
        value.YY_MOVE_OR_COPY<Definition::Ptr>(YY_MOVE(that.value));
        break;

      case symbol_kind::S_Pattern: // Pattern
        value.YY_MOVE_OR_COPY<Pattern::Ptr>(YY_MOVE(that.value));
        break;

      case symbol_kind::S_INT: // INT
        value.YY_MOVE_OR_COPY<int>(YY_MOVE(that.value));
        break;

      case symbol_kind::S_LID: // LID
      case symbol_kind::S_UID: // UID
        value.YY_MOVE_OR_COPY<std::string>(YY_MOVE(that.value));
        break;

      case symbol_kind::S_Branches: // Branches
        value.YY_MOVE_OR_COPY<std::vector<Branch::Ptr> >(YY_MOVE(that.value));
        break;

      case symbol_kind::S_Constructors: // Constructors
        value.YY_MOVE_OR_COPY<std::vector<Constructor::Ptr> >(
          YY_MOVE(that.value));
        break;

      case symbol_kind::S_Program:     // Program
      case symbol_kind::S_Definitions: // Definitions
        value.YY_MOVE_OR_COPY<std::vector<Definition::Ptr> >(
          YY_MOVE(that.value));
        break;

      case symbol_kind::S_LowercaseParams: // LowercaseParams
      case symbol_kind::S_UppercaseParams: // UppercaseParams
        value.YY_MOVE_OR_COPY<std::vector<std::string> >(YY_MOVE(that.value));
        break;

      default:
        break;
      }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type(
    state_type s, YY_MOVE_REF(symbol_type) that)
    : super_type(s, YY_MOVE(that.location))
  {
    switch (that.kind())
      {
      case symbol_kind::S_Add:             // Add
      case symbol_kind::S_Mul:             // Mul
      case symbol_kind::S_Application:     // Application
      case symbol_kind::S_ApplicationBase: // ApplicationBase
      case symbol_kind::S_Match:           // Match
        value.move<AST::Ptr>(YY_MOVE(that.value));
        break;

      case symbol_kind::S_Branch: // Branch
        value.move<Branch::Ptr>(YY_MOVE(that.value));
        break;

      case symbol_kind::S_Constructor: // Constructor
        value.move<Constructor::Ptr>(YY_MOVE(that.value));
        break;

      case symbol_kind::S_Definition: // Definition
      case symbol_kind::S_Fn:         // Fn
      case symbol_kind::S_Data:       // Data
        value.move<Definition::Ptr>(YY_MOVE(that.value));
        break;

      case symbol_kind::S_Pattern: // Pattern
        value.move<Pattern::Ptr>(YY_MOVE(that.value));
        break;

      case symbol_kind::S_INT: // INT
        value.move<int>(YY_MOVE(that.value));
        break;

      case symbol_kind::S_LID: // LID
      case symbol_kind::S_UID: // UID
        value.move<std::string>(YY_MOVE(that.value));
        break;

      case symbol_kind::S_Branches: // Branches
        value.move<std::vector<Branch::Ptr> >(YY_MOVE(that.value));
        break;

      case symbol_kind::S_Constructors: // Constructors
        value.move<std::vector<Constructor::Ptr> >(YY_MOVE(that.value));
        break;

      case symbol_kind::S_Program:     // Program
      case symbol_kind::S_Definitions: // Definitions
        value.move<std::vector<Definition::Ptr> >(YY_MOVE(that.value));
        break;

      case symbol_kind::S_LowercaseParams: // LowercaseParams
      case symbol_kind::S_UppercaseParams: // UppercaseParams
        value.move<std::vector<std::string> >(YY_MOVE(that.value));
        break;

      default:
        break;
      }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
    parser::stack_symbol_type::operator=(const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind())
      {
      case symbol_kind::S_Add:             // Add
      case symbol_kind::S_Mul:             // Mul
      case symbol_kind::S_Application:     // Application
      case symbol_kind::S_ApplicationBase: // ApplicationBase
      case symbol_kind::S_Match:           // Match
        value.copy<AST::Ptr>(that.value);
        break;

      case symbol_kind::S_Branch: // Branch
        value.copy<Branch::Ptr>(that.value);
        break;

      case symbol_kind::S_Constructor: // Constructor
        value.copy<Constructor::Ptr>(that.value);
        break;

      case symbol_kind::S_Definition: // Definition
      case symbol_kind::S_Fn:         // Fn
      case symbol_kind::S_Data:       // Data
        value.copy<Definition::Ptr>(that.value);
        break;

      case symbol_kind::S_Pattern: // Pattern
        value.copy<Pattern::Ptr>(that.value);
        break;

      case symbol_kind::S_INT: // INT
        value.copy<int>(that.value);
        break;

      case symbol_kind::S_LID: // LID
      case symbol_kind::S_UID: // UID
        value.copy<std::string>(that.value);
        break;

      case symbol_kind::S_Branches: // Branches
        value.copy<std::vector<Branch::Ptr> >(that.value);
        break;

      case symbol_kind::S_Constructors: // Constructors
        value.copy<std::vector<Constructor::Ptr> >(that.value);
        break;

      case symbol_kind::S_Program:     // Program
      case symbol_kind::S_Definitions: // Definitions
        value.copy<std::vector<Definition::Ptr> >(that.value);
        break;

      case symbol_kind::S_LowercaseParams: // LowercaseParams
      case symbol_kind::S_UppercaseParams: // UppercaseParams
        value.copy<std::vector<std::string> >(that.value);
        break;

      default:
        break;
      }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
    parser::stack_symbol_type::operator=(stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind())
      {
      case symbol_kind::S_Add:             // Add
      case symbol_kind::S_Mul:             // Mul
      case symbol_kind::S_Application:     // Application
      case symbol_kind::S_ApplicationBase: // ApplicationBase
      case symbol_kind::S_Match:           // Match
        value.move<AST::Ptr>(that.value);
        break;

      case symbol_kind::S_Branch: // Branch
        value.move<Branch::Ptr>(that.value);
        break;

      case symbol_kind::S_Constructor: // Constructor
        value.move<Constructor::Ptr>(that.value);
        break;

      case symbol_kind::S_Definition: // Definition
      case symbol_kind::S_Fn:         // Fn
      case symbol_kind::S_Data:       // Data
        value.move<Definition::Ptr>(that.value);
        break;

      case symbol_kind::S_Pattern: // Pattern
        value.move<Pattern::Ptr>(that.value);
        break;

      case symbol_kind::S_INT: // INT
        value.move<int>(that.value);
        break;

      case symbol_kind::S_LID: // LID
      case symbol_kind::S_UID: // UID
        value.move<std::string>(that.value);
        break;

      case symbol_kind::S_Branches: // Branches
        value.move<std::vector<Branch::Ptr> >(that.value);
        break;

      case symbol_kind::S_Constructors: // Constructors
        value.move<std::vector<Constructor::Ptr> >(that.value);
        break;

      case symbol_kind::S_Program:     // Program
      case symbol_kind::S_Definitions: // Definitions
        value.move<std::vector<Definition::Ptr> >(that.value);
        break;

      case symbol_kind::S_LowercaseParams: // LowercaseParams
      case symbol_kind::S_UppercaseParams: // UppercaseParams
        value.move<std::vector<std::string> >(that.value);
        break;

      default:
        break;
      }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void parser::yy_destroy_(const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT(yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
    parser::yy_print_(std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE(yyoutput);
    if (yysym.empty())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm") << ' ' << yysym.name()
            << " (" << yysym.location << ": ";
        YY_USE(yykind);
        yyo << ')';
      }
  }
#endif

  void parser::yypush_(const char* m, YY_MOVE_REF(stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT(m, sym);
    yystack_.push(YY_MOVE(sym));
  }

  void parser::yypush_(const char* m, state_type s, YY_MOVE_REF(symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_(m, stack_symbol_type(s, std::move(sym)));
#else
    stack_symbol_type ss(s, sym);
    yypush_(m, ss);
#endif
  }

  void parser::yypop_(int n) YY_NOEXCEPT { yystack_.pop(n); }

#if YYDEBUG
  std::ostream& parser::debug_stream() const { return *yycdebug_; }

  void parser::set_debug_stream(std::ostream& o) { yycdebug_ = &o; }

  parser::debug_level_type parser::debug_level() const { return yydebug_; }

  void parser::set_debug_level(debug_level_type l) { yydebug_ = l; }
#endif // YYDEBUG

  parser::state_type parser::yy_lr_goto_state_(state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool parser::yy_pact_value_is_default_(int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool parser::yy_table_value_is_error_(int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int parser::operator()() { return parse(); }

  int parser::parse()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
        YYCDEBUG << "Starting parse\n";

        /* Initialize the stack.  The initial state will be set in
           yynewstate, since the latter expects the semantical and the
           location values to have been already stored, initialize these
           stacks with a primary value.  */
        yystack_.clear();
        yypush_(YY_NULLPTR, 0, YY_MOVE(yyla));

      /*-----------------------------------------------.
      | yynewstate -- push a new symbol on the stack.  |
      `-----------------------------------------------*/
      yynewstate:
        YYCDEBUG << "Entering state " << int(yystack_[0].state) << '\n';
        YY_STACK_PRINT();

        // Accept?
        if (yystack_[0].state == yyfinal_)
          YYACCEPT;

        goto yybackup;

      /*-----------.
      | yybackup.  |
      `-----------*/
      yybackup:
        // Try to take a decision without lookahead.
        yyn = yypact_[+yystack_[0].state];
        if (yy_pact_value_is_default_(yyn))
          goto yydefault;

        // Read a lookahead token.
        if (yyla.empty())
          {
            YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
            try
#endif // YY_EXCEPTIONS
              {
                symbol_type yylookahead(yylex());
                yyla.move(yylookahead);
              }
#if YY_EXCEPTIONS
            catch (const syntax_error& yyexc)
              {
                YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
                error(yyexc);
                goto yyerrlab1;
              }
#endif // YY_EXCEPTIONS
          }
        YY_SYMBOL_PRINT("Next token is", yyla);

        if (yyla.kind() == symbol_kind::S_YYerror)
          {
            // The scanner already issued an error message, process directly
            // to error recovery.  But do not keep the error token as
            // lookahead, it is too special and may lead us to an endless
            // loop in error recovery. */
            yyla.kind_ = symbol_kind::S_YYUNDEF;
            goto yyerrlab1;
          }

        /* If the proper action on seeing token YYLA.TYPE is to reduce or
           to detect an error, take that action.  */
        yyn += yyla.kind();
        if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind())
          goto yydefault;

        // Reduce or error.
        yyn = yytable_[yyn];
        if (yyn <= 0)
          {
            if (yy_table_value_is_error_(yyn))
              goto yyerrlab;
            yyn = -yyn;
            goto yyreduce;
          }

        // Count tokens shifted since error; after three, turn off error status.
        if (yyerrstatus_)
          --yyerrstatus_;

        // Shift the lookahead token.
        yypush_("Shifting", state_type(yyn), YY_MOVE(yyla));
        goto yynewstate;

      /*-----------------------------------------------------------.
      | yydefault -- do the default action for the current state.  |
      `-----------------------------------------------------------*/
      yydefault:
        yyn = yydefact_[+yystack_[0].state];
        if (yyn == 0)
          goto yyerrlab;
        goto yyreduce;

      /*-----------------------------.
      | yyreduce -- do a reduction.  |
      `-----------------------------*/
      yyreduce:
        yylen = yyr2_[yyn];
        {
          stack_symbol_type yylhs;
          yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
          /* Variants are always initialized to an empty instance of the
             correct type. The default '$$ = $1' action is NOT applied
             when using variants.  */
          switch (yyr1_[yyn])
            {
            case symbol_kind::S_Add:             // Add
            case symbol_kind::S_Mul:             // Mul
            case symbol_kind::S_Application:     // Application
            case symbol_kind::S_ApplicationBase: // ApplicationBase
            case symbol_kind::S_Match:           // Match
              yylhs.value.emplace<AST::Ptr>();
              break;

            case symbol_kind::S_Branch: // Branch
              yylhs.value.emplace<Branch::Ptr>();
              break;

            case symbol_kind::S_Constructor: // Constructor
              yylhs.value.emplace<Constructor::Ptr>();
              break;

            case symbol_kind::S_Definition: // Definition
            case symbol_kind::S_Fn:         // Fn
            case symbol_kind::S_Data:       // Data
              yylhs.value.emplace<Definition::Ptr>();
              break;

            case symbol_kind::S_Pattern: // Pattern
              yylhs.value.emplace<Pattern::Ptr>();
              break;

            case symbol_kind::S_INT: // INT
              yylhs.value.emplace<int>();
              break;

            case symbol_kind::S_LID: // LID
            case symbol_kind::S_UID: // UID
              yylhs.value.emplace<std::string>();
              break;

            case symbol_kind::S_Branches: // Branches
              yylhs.value.emplace<std::vector<Branch::Ptr> >();
              break;

            case symbol_kind::S_Constructors: // Constructors
              yylhs.value.emplace<std::vector<Constructor::Ptr> >();
              break;

            case symbol_kind::S_Program:     // Program
            case symbol_kind::S_Definitions: // Definitions
              yylhs.value.emplace<std::vector<Definition::Ptr> >();
              break;

            case symbol_kind::S_LowercaseParams: // LowercaseParams
            case symbol_kind::S_UppercaseParams: // UppercaseParams
              yylhs.value.emplace<std::vector<std::string> >();
              break;

            default:
              break;
            }

          // Default location.
          {
            stack_type::slice range(yystack_, yylen);
            YYLLOC_DEFAULT(yylhs.location, range, yylen);
            yyerror_range[1].location = yylhs.location;
          }

          // Perform the reduction.
          YY_REDUCE_PRINT(yyn);
#if YY_EXCEPTIONS
          try
#endif // YY_EXCEPTIONS
            {
              switch (yyn)
                {
                  case 2: // Program: Definitions
#line 64 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    Program = std::move(
                      yystack_[0].value.as<std::vector<Definition::Ptr> >());
                  }
#line 808 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 3: // Definitions: Definitions Definition
#line 68 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<std::vector<Definition::Ptr> >() = std::move(
                      yystack_[1].value.as<std::vector<Definition::Ptr> >());
                    yylhs.value.as<std::vector<Definition::Ptr> >().push_back(
                      std::move(yystack_[0].value.as<Definition::Ptr>()));
                  }
#line 814 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 4: // Definitions: Definition
#line 69 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<std::vector<Definition::Ptr> >() =
                      std::vector<Definition::Ptr>();
                    yylhs.value.as<std::vector<Definition::Ptr> >().push_back(
                      std::move(yystack_[0].value.as<Definition::Ptr>()));
                  }
#line 820 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 5: // Definition: Fn
#line 73 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<Definition::Ptr>() =
                      std::move(yystack_[0].value.as<Definition::Ptr>());
                  }
#line 826 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 6: // Definition: Data
#line 74 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<Definition::Ptr>() =
                      std::move(yystack_[0].value.as<Definition::Ptr>());
                  }
#line 832 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 7: // Fn: FN LID OPAREN LowercaseParams CPAREN EQUAL
                          // OCURLY Add CCURLY
#line 79 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<Definition::Ptr>() = Definition::Ptr(new Fn(
                      yylhs.location,
                      std::move(yystack_[7].value.as<std::string>()),
                      std::move(
                        yystack_[5].value.as<std::vector<std::string> >()),
                      std::move(yystack_[1].value.as<AST::Ptr>())));
                  }
#line 838 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 8: // LowercaseParams: %empty
#line 84 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<std::vector<std::string> >() =
                      std::vector<std::string>();
                  }
#line 844 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 9: // LowercaseParams: LID
#line 85 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<std::vector<std::string> >().push_back(
                      std::move(yystack_[0].value.as<std::string>()));
                  }
#line 850 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 10: // LowercaseParams: LowercaseParams COMMA LID
#line 86 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<std::vector<std::string> >() = std::move(
                      yystack_[2].value.as<std::vector<std::string> >());
                    yylhs.value.as<std::vector<std::string> >().push_back(
                      std::move(yystack_[0].value.as<std::string>()));
                  }
#line 856 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 11: // UppercaseParams: %empty
#line 90 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<std::vector<std::string> >() =
                      std::vector<std::string>();
                  }
#line 862 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 12: // UppercaseParams: UppercaseParams UID
#line 91 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<std::vector<std::string> >() = std::move(
                      yystack_[1].value.as<std::vector<std::string> >());
                    yylhs.value.as<std::vector<std::string> >().push_back(
                      std::move(yystack_[0].value.as<std::string>()));
                  }
#line 868 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 13: // Add: Add PLUS Mul
#line 95 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<AST::Ptr>() = AST::Ptr(new Binop(
                      yylhs.location,
                      utils::Binop::PLUS,
                      std::move(yystack_[2].value.as<AST::Ptr>()),
                      std::move(yystack_[0].value.as<AST::Ptr>())));
                  }
#line 874 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 14: // Add: Add MINUS Mul
#line 96 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<AST::Ptr>() = AST::Ptr(new Binop(
                      yylhs.location,
                      utils::Binop::MINUS,
                      std::move(yystack_[2].value.as<AST::Ptr>()),
                      std::move(yystack_[0].value.as<AST::Ptr>())));
                  }
#line 880 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 15: // Add: Mul
#line 97 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<AST::Ptr>() =
                      std::move(yystack_[0].value.as<AST::Ptr>());
                  }
#line 886 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 16: // Mul: Mul TIMES Application
#line 101 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<AST::Ptr>() = AST::Ptr(new Binop(
                      yylhs.location,
                      utils::Binop::TIMES,
                      std::move(yystack_[2].value.as<AST::Ptr>()),
                      std::move(yystack_[0].value.as<AST::Ptr>())));
                  }
#line 892 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 17: // Mul: Mul DIVIDE Application
#line 102 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<AST::Ptr>() = AST::Ptr(new Binop(
                      yylhs.location,
                      utils::Binop::DIVIDE,
                      std::move(yystack_[2].value.as<AST::Ptr>()),
                      std::move(yystack_[0].value.as<AST::Ptr>())));
                  }
#line 898 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 18: // Mul: Application
#line 103 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<AST::Ptr>() =
                      std::move(yystack_[0].value.as<AST::Ptr>());
                  }
#line 904 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 19: // Application: Application ApplicationBase
#line 107 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<AST::Ptr>() = AST::Ptr(new Application(
                      yylhs.location,
                      std::move(yystack_[1].value.as<AST::Ptr>()),
                      std::move(yystack_[0].value.as<AST::Ptr>())));
                  }
#line 910 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 20: // Application: ApplicationBase
#line 108 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<AST::Ptr>() =
                      std::move(yystack_[0].value.as<AST::Ptr>());
                  }
#line 916 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 21: // ApplicationBase: INT
#line 112 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<AST::Ptr>() = AST::Ptr(
                      new Int(yylhs.location, yystack_[0].value.as<int>()));
                  }
#line 922 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 22: // ApplicationBase: LID
#line 113 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<AST::Ptr>() = AST::Ptr(new LID(
                      yylhs.location,
                      std::move(yystack_[0].value.as<std::string>())));
                  }
#line 928 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 23: // ApplicationBase: UID
#line 114 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<AST::Ptr>() = AST::Ptr(new UID(
                      yylhs.location,
                      std::move(yystack_[0].value.as<std::string>())));
                  }
#line 934 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 24: // ApplicationBase: OPAREN Add CPAREN
#line 115 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<AST::Ptr>() =
                      std::move(yystack_[1].value.as<AST::Ptr>());
                  }
#line 940 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 25: // ApplicationBase: Match
#line 116 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<AST::Ptr>() =
                      std::move(yystack_[0].value.as<AST::Ptr>());
                  }
#line 946 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 26: // Match: MATCH Add WITH OCURLY Branches CCURLY
#line 121 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<AST::Ptr>() = AST::Ptr(new Match(
                      yylhs.location,
                      std::move(yystack_[4].value.as<AST::Ptr>()),
                      std::move(
                        yystack_[1].value.as<std::vector<Branch::Ptr> >())));
                  }
#line 952 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 27: // Branches: Branches Branch
#line 125 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<std::vector<Branch::Ptr> >() = std::move(
                      yystack_[1].value.as<std::vector<Branch::Ptr> >());
                    yylhs.value.as<std::vector<Branch::Ptr> >().push_back(
                      std::move(yystack_[0].value.as<Branch::Ptr>()));
                  }
#line 958 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 28: // Branches: Branch
#line 126 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<std::vector<Branch::Ptr> >() =
                      std::vector<Branch::Ptr>();
                    yylhs.value.as<std::vector<Branch::Ptr> >().push_back(
                      std::move(yystack_[0].value.as<Branch::Ptr>()));
                  }
#line 964 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 29: // Branch: VERTIAL Pattern DOUBLEARROW OCURLY Add
                           // CCURLY
#line 131 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<Branch::Ptr>() = Branch::Ptr(new Branch(
                      yylhs.location,
                      std::move(yystack_[4].value.as<Pattern::Ptr>()),
                      std::move(yystack_[1].value.as<AST::Ptr>())));
                  }
#line 970 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 30: // Pattern: LID
#line 135 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<Pattern::Ptr>() =
                      Pattern::Ptr(new VariablePattern(
                        yylhs.location,
                        std::move(yystack_[0].value.as<std::string>())));
                  }
#line 976 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 31: // Pattern: UID
#line 136 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<Pattern::Ptr>() =
                      Pattern::Ptr(new ConstructorPattern(
                        yylhs.location,
                        std::move(yystack_[0].value.as<std::string>()),
                        std::vector<std::string>()));
                  }
#line 982 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 32: // Pattern: UID OPAREN LowercaseParams CPAREN
#line 138 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<Pattern::Ptr>() =
                      Pattern::Ptr(new ConstructorPattern(
                        yylhs.location,
                        std::move(yystack_[3].value.as<std::string>()),
                        std::move(
                          yystack_[1].value.as<std::vector<std::string> >())));
                  }
#line 988 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 33: // Data: DATA UID EQUAL OBRACKET Constructors
                           // CBRACKET
#line 143 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<Definition::Ptr>() =
                      Definition::Ptr(new Data(
                        yylhs.location,
                        std::move(yystack_[4].value.as<std::string>()),
                        std::move(
                          yystack_[1]
                            .value.as<std::vector<Constructor::Ptr> >())));
                  }
#line 994 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 34: // Constructors: Constructors COMMA Constructor
#line 147 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<std::vector<Constructor::Ptr> >() =
                      std::move(
                        yystack_[2].value.as<std::vector<Constructor::Ptr> >());
                    yylhs.value.as<std::vector<Constructor::Ptr> >().push_back(
                      std::move(yystack_[0].value.as<Constructor::Ptr>()));
                  }
#line 1000 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 35: // Constructors: Constructor
#line 149 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<std::vector<Constructor::Ptr> >() =
                      std::vector<Constructor::Ptr>();
                    yylhs.value.as<std::vector<Constructor::Ptr> >().push_back(
                      std::move(yystack_[0].value.as<Constructor::Ptr>()));
                  }
#line 1006 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

                  case 36: // Constructor: UID UppercaseParams
#line 154 "/home/muqiu/Workspace/Swallow/compiler/parser/parser.y"
                  {
                    yylhs.value.as<Constructor::Ptr>() =
                      Constructor::Ptr(new Constructor(
                        yylhs.location,
                        std::move(yystack_[1].value.as<std::string>()),
                        std::move(
                          yystack_[0].value.as<std::vector<std::string> >())));
                  }
#line 1012 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
                  break;

#line 1016 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"

                default:
                  break;
                }
            }
#if YY_EXCEPTIONS
          catch (const syntax_error& yyexc)
            {
              YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
              error(yyexc);
              YYERROR;
            }
#endif // YY_EXCEPTIONS
          YY_SYMBOL_PRINT("-> $$ =", yylhs);
          yypop_(yylen);
          yylen = 0;

          // Shift the result of the reduction.
          yypush_(YY_NULLPTR, YY_MOVE(yylhs));
        }
        goto yynewstate;

      /*--------------------------------------.
      | yyerrlab -- here on detecting error.  |
      `--------------------------------------*/
      yyerrlab:
        // If not already recovering from an error, report this error.
        if (!yyerrstatus_)
          {
            ++yynerrs_;
            std::string msg = YY_("syntax error");
            error(yyla.location, YY_MOVE(msg));
          }

        yyerror_range[1].location = yyla.location;
        if (yyerrstatus_ == 3)
          {
            /* If just tried and failed to reuse lookahead token after an
               error, discard it.  */

            // Return failure if at end of input.
            if (yyla.kind() == symbol_kind::S_YYEOF)
              YYABORT;
            else if (!yyla.empty())
              {
                yy_destroy_("Error: discarding", yyla);
                yyla.clear();
              }
          }

        // Else will try to reuse lookahead token after shifting the error
        // token.
        goto yyerrlab1;

      /*---------------------------------------------------.
      | yyerrorlab -- error raised explicitly by YYERROR.  |
      `---------------------------------------------------*/
      yyerrorlab:
        /* Pacify compilers when the user code never invokes YYERROR and
           the label yyerrorlab therefore never appears in user code.  */
        if (false)
          YYERROR;

        /* Do not reclaim the symbols of the rule whose action triggered
           this YYERROR.  */
        yypop_(yylen);
        yylen = 0;
        YY_STACK_PRINT();
        goto yyerrlab1;

      /*-------------------------------------------------------------.
      | yyerrlab1 -- common code for both syntax error and YYERROR.  |
      `-------------------------------------------------------------*/
      yyerrlab1:
        yyerrstatus_ = 3; // Each real token shifted decrements this.
        // Pop stack until we find a state that shifts the error token.
        for (;;)
          {
            yyn = yypact_[+yystack_[0].state];
            if (!yy_pact_value_is_default_(yyn))
              {
                yyn += symbol_kind::S_YYerror;
                if (
                  0 <= yyn && yyn <= yylast_
                  && yycheck_[yyn] == symbol_kind::S_YYerror)
                  {
                    yyn = yytable_[yyn];
                    if (0 < yyn)
                      break;
                  }
              }

            // Pop the current state because it cannot handle the error token.
            if (yystack_.size() == 1)
              YYABORT;

            yyerror_range[1].location = yystack_[0].location;
            yy_destroy_("Error: popping", yystack_[0]);
            yypop_();
            YY_STACK_PRINT();
          }
        {
          stack_symbol_type error_token;

          yyerror_range[2].location = yyla.location;
          YYLLOC_DEFAULT(error_token.location, yyerror_range, 2);

          // Shift the error token.
          error_token.state = state_type(yyn);
          yypush_("Shifting", YY_MOVE(error_token));
        }
        goto yynewstate;

      /*-------------------------------------.
      | yyacceptlab -- YYACCEPT comes here.  |
      `-------------------------------------*/
      yyacceptlab:
        yyresult = 0;
        goto yyreturn;

      /*-----------------------------------.
      | yyabortlab -- YYABORT comes here.  |
      `-----------------------------------*/
      yyabortlab:
        yyresult = 1;
        goto yyreturn;

      /*-----------------------------------------------------.
      | yyreturn -- parsing is finished, return the result.  |
      `-----------------------------------------------------*/
      yyreturn:
        if (!yyla.empty())
          yy_destroy_("Cleanup: discarding lookahead", yyla);

        /* Do not reclaim the symbols of the rule whose action triggered
           this YYABORT or YYACCEPT.  */
        yypop_(yylen);
        YY_STACK_PRINT();
        while (1 < yystack_.size())
          {
            yy_destroy_("Cleanup: popping", yystack_[0]);
            yypop_();
          }

        return yyresult;
      }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty())
          yy_destroy_(YY_NULLPTR, yyla);

        while (1 < yystack_.size())
          {
            yy_destroy_(YY_NULLPTR, yystack_[0]);
            yypop_();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void parser::error(const syntax_error& yyexc)
  {
    error(yyexc.location, yyexc.what());
  }

#if YYDEBUG || 0
  const char* parser::symbol_name(symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0

  const signed char parser::yypact_ninf_ = -32;

  const signed char parser::yytable_ninf_ = -1;

  const signed char parser::yypact_[] = {
    28,  -21, 0,   29, 28, -32, -32, -32, 19, 15,  -32, -32, 7,  27,
    -32, -3,  25,  23, 30, -32, 22,  -32, 35, -32, 26,  -32, 25, -5,
    -32, -32, -32, -5, -5, -32, -32, 5,   24, -5,  -32, -32, 21, 1,
    -5,  -5,  -32, -5, -5, -32, 40,  -32, 24, 24,  -5,  -5,  33, 17,
    14,  -32, -32, 41, 34, -32, -32, 7,   45, -1,  -5,  -32, 8,  -32};

  const signed char parser::yydefact_[] = {
    0,  0,  0,  0,  2,  4,  5,  6,  0,  0, 1,  3,  8,  0,  9,  0,  0,  0,
    0,  11, 0,  35, 0,  10, 36, 33, 0,  0, 12, 34, 21, 0,  0,  22, 23, 0,
    15, 18, 20, 25, 0,  0,  0,  0,  7,  0, 0,  19, 0,  24, 13, 14, 16, 17,
    0,  0,  0,  28, 30, 31, 0,  26, 27, 8, 0,  0,  0,  32, 0,  29};

  const signed char parser::yypgoto_[] = {
    -32,
    -32,
    -32,
    54,
    -32,
    -4,
    -32,
    -31,
    2,
    3,
    -30,
    -32,
    -32,
    4,
    -32,
    -32,
    -32,
    36};

  const signed char parser::yydefgoto_[] = {
    0, 3, 4, 5, 6, 15, 24, 35, 36, 37, 38, 39, 56, 57, 60, 7, 20, 21};

  const signed char parser::yytable_[] = {
    40, 41, 30, 8,  42, 31, 43, 47, 42, 32, 43, 42, 17, 43, 67, 18,
    49, 18, 44, 33, 34, 69, 47, 47, 42, 9,  43, 61, 45, 10, 46, 14,
    48, 12, 55, 68, 1,  2,  13, 25, 26, 58, 59, 16, 50, 51, 22, 27,
    52, 53, 19, 28, 54, 55, 23, 63, 64, 66, 11, 65, 62, 0,  29};

  const signed char parser::yycheck_[] = {
    31, 32, 7,  24, 3,  10, 5,  37, 3,  14, 5,  3,  15, 5,  15, 18,
    15, 18, 13, 24, 25, 13, 52, 53, 3,  25, 5,  13, 4,  0,  6,  24,
    11, 14, 20, 66, 8,  9,  23, 17, 18, 24, 25, 16, 42, 43, 23, 12,
    45, 46, 25, 25, 12, 20, 24, 14, 22, 12, 4,  63, 56, -1, 26};

  const signed char parser::yystos_[] = {
    0,  8,  9,  27, 28, 29, 30, 41, 24, 25, 0,  29, 14, 23, 24, 31, 16, 15,
    18, 25, 42, 43, 23, 24, 32, 17, 18, 12, 25, 43, 7,  10, 14, 24, 25, 33,
    34, 35, 36, 37, 33, 33, 3,  5,  13, 4,  6,  36, 11, 15, 34, 34, 35, 35,
    12, 20, 38, 39, 24, 25, 40, 13, 39, 14, 22, 31, 12, 15, 33, 13};

  const signed char parser::yyr1_[] = {
    0,  26, 27, 28, 28, 29, 29, 30, 31, 31, 31, 32, 32, 33, 33, 33, 34, 34, 34,
    35, 35, 36, 36, 36, 36, 36, 37, 38, 38, 39, 40, 40, 40, 41, 42, 42, 43};

  const signed char parser::yyr2_[] = {0, 2, 1, 2, 1, 1, 1, 9, 0, 1, 3, 0, 2,
                                       3, 3, 1, 3, 3, 1, 2, 1, 1, 1, 1, 3, 1,
                                       6, 2, 1, 6, 1, 1, 4, 6, 3, 1, 2};

#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char* const parser::yytname_[] = {
    "\"end of file\"",
    "error",
    "\"invalid token\"",
    "PLUS",
    "TIMES",
    "MINUS",
    "DIVIDE",
    "INT",
    "FN",
    "DATA",
    "MATCH",
    "WITH",
    "OCURLY",
    "CCURLY",
    "OPAREN",
    "CPAREN",
    "OBRACKET",
    "CBRACKET",
    "COMMA",
    "ARROW",
    "VERTIAL",
    "SEMICOLON",
    "DOUBLEARROW",
    "EQUAL",
    "LID",
    "UID",
    "$accept",
    "Program",
    "Definitions",
    "Definition",
    "Fn",
    "LowercaseParams",
    "UppercaseParams",
    "Add",
    "Mul",
    "Application",
    "ApplicationBase",
    "Match",
    "Branches",
    "Branch",
    "Pattern",
    "Data",
    "Constructors",
    "Constructor",
    YY_NULLPTR};
#endif

#if YYDEBUG
  const unsigned char parser::yyrline_[] = {
    0,   64,  64,  68,  69,  73,  74,  78,  84,  85,  86,  90,  91,
    95,  96,  97,  101, 102, 103, 107, 108, 112, 113, 114, 115, 116,
    120, 125, 126, 130, 135, 136, 137, 142, 147, 148, 153};

  void parser::yy_stack_print_() const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator i = yystack_.begin(),
                                    i_end = yystack_.end();
         i != i_end;
         ++i)
      *yycdebug_ << ' ' << int(i->state);
    *yycdebug_ << '\n';
  }

  void parser::yy_reduce_print_(int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1 << " (line " << yylno
               << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT(
        "   $" << yyi + 1 << " =", yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

} // namespace yy

#line 1363 "/home/muqiu/Workspace/Swallow/compiler/parser/bison_parser.cpp"
