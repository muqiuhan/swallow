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

#ifndef SWALLOW_COMPILER_DIAGNOSTICS_CODE_HPP
#define SWALLOW_COMPILER_DIAGNOSTICS_CODE_HPP

#include <cstdint>
namespace swallow::compiler::diagnostics
{

  enum Code : uint32_t
  {
    UNKNOWN,
    PARSING,
    LID_NOT_DECLARED,
    UID_NOT_DECLARED,
    EXPR_TYPE_CHECKING_FAILED,
    BINOP_NOT_DECLARED,
    BINOP_TYPE_MISMATCH,
    APPLICATION_TYPE_MISMATCH,
    MATCH_EXPR_TYPE_CHECKING_FAILED,
    MATCH_EXPR_BRANCHE_TYPE_CONFLICTS,
    MATCH_EXPR_CURRENT_BRANCHE_TYPE_CHECKING_FAILED,
    MATCH_EXPR_IS_NON_EXHAUSTIVE,
    MATCH_NON_DATA_TYPE,
    PATTERN_CONSTRUCTOR_IS_NOT_FUNCTION,
    PATTERN_CONSTRUCTOR_IS_DUPLICATED,
    PATTERN_MISMATCH,
    FUNCTION_BODY_TYPE_CHECKING_FAILED,
    CONSTRUCTOR_NOT_DECLARED,
    AMBIGUOUSLY_TYPE,
  };

}

#endif /* SWALLOW_COMPILER_DIAGNOSTICS_CODE_HPP */