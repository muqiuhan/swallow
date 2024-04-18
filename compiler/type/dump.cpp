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

#include "environment.hpp"
#include "type.hpp"
#include <ostream>

namespace swallow::compiler::type
{
  void
    Variable::Dump(const Manager &typeManager, std::ostream &to) const noexcept
  {
    const auto it = typeManager.Types.find(Name);

    if (it != typeManager.Types.end())
      it->second->Dump(typeManager, to);
    else
      to << Name;
  }

  void Arrow::Dump(const Manager &typeManager, std::ostream &to) const noexcept
  {
    Left->Dump(typeManager, to);
    to << " -> ";
    Left->Dump(typeManager, to);
  }

  void Base::Dump(const Manager &typeManager, std::ostream &to) const noexcept
  {
    to << Name;
  }

  void Environment::Dump(std::ostream &to, const Manager &typeManager) noexcept
  {
    for (const auto &[name, type] : Names)
      {
        to << name << ": ";
        type->Dump(typeManager, to);
        to << '\n';
      }
  }
} // namespace swallow::compiler::type