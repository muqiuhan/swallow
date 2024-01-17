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
//     * Neither the name of Terifo nor the names of its contributors
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

#include "type.h"
#include "../utils/panic/panic.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <format>
#include <iterator>
#include <ostream>

using namespace swallow::utils;

namespace swallow::type {
std::string Manager::newTypeName() noexcept {
  int32_t currentID = LastID++;
  std::string name;

  while (currentID != -1) {
    name += static_cast<char>(('a' + (currentID % 26)));
    currentID = currentID / 26 - 1;
  }

  std::reverse(name.begin(), name.end());
  return name;
}

Type::Ptr Manager::newType() noexcept {
  return Type::Ptr(new Variable(newTypeName()));
}

Type::Ptr Manager::newArrowType() noexcept {
  return Type::Ptr(new Arrow(newType(), newType()));
}

Type::Ptr Manager::resolve(Type::Ptr type, Variable *&var) noexcept {
  Variable *cast;
  var = nullptr;

  while ((cast = dynamic_cast<Variable *>(type.get()))) {
    const auto it = Types.find(cast->Name);
    if (it == Types.end()) {
      var = cast;
      break;
    }

    type = it->second;
  }
  return type;
}

/* clang-format off
-------------------------------------------------------------
(lldb) r
Process 12894 launched: '/home/muqiuhan/Workspace/swallow/build/linux/x86_64/debug/swc' (x86_64)
let add x y = { x + y }

let double x = { add x x }

let main argv = { double 163 }
Type checking...
Bind binary operators...
scan definitions type...
scaning function 'add' definition
scaning function 'double' definition
scaning function 'main' definition
type checking...
checking function 'add' type
compiler/type/type.cpp:101 panic: type checking failed: unification failed

terminate called without an active exception
Process 12894 stopped
* thread #1, name = 'swc', stop reason = signal SIGABRT
    frame #0: 0x00007ffff7891dec libc.so.6`__pthread_kill_implementation + 270
libc.so.6`__pthread_kill_implementation:
->  0x7ffff7891dec <+270>: movl   %eax, %ebx
    0x7ffff7891dee <+272>: negl   %ebx
    0x7ffff7891df0 <+274>: cmpl   $0xfffff000, %eax         ; imm = 0xFFFFF000 
    0x7ffff7891df5 <+279>: movl   $0x0, %eax
(lldb) bt
* thread #1, name = 'swc', stop reason = signal SIGABRT
  * frame #0: 0x00007ffff7891dec libc.so.6`__pthread_kill_implementation + 270
    frame #1: 0x00007ffff783f0c6 libc.so.6`raise + 24
    frame #2: 0x00007ffff78268d7 libc.so.6`abort + 217
    frame #3: 0x00007ffff7cacc37 libstdc++.so.6`___lldb_unnamed_symbol7354 + 96
    frame #4: 0x00007ffff7cbc21c libstdc++.so.6`___lldb_unnamed_symbol7803 + 12
    frame #5: 0x00007ffff7cbc287 libstdc++.so.6`std::terminate() + 23
    frame #6: 0x000000000040bcdf swc`void swallow::utils::panic<>(fmt=panic_format<> @ 0x00007fffffffcf50) at panic.hpp:58:17
    frame #7: 0x0000000000464e0d swc`swallow::type::Manager::unify(this=0x00007fffffffd200, left=nullptr, right=std::__shared_ptr<swallow::type::Type, __gnu_cxx::_S_atomic>::element_type @ 0x00000000004ac6c0) at type.cpp:101:10
    frame #8: 0x0000000000407a14 swc`swallow::ast::Fn::typecheck(this=0x00000000004b2db0, typeManager=0x00007fffffffd200, typeEnvironment=0x00007fffffffd1c0) const at ast.cpp:188:20
    frame #9: 0x00000000004082da swc`swallow::type::typecheck(program=size=3) at ast.cpp:233:26
    frame #10: 0x000000000040689a swc`main(argc=1, argv=0x00007fffffffd548) at compiler.cpp:31:18
    frame #11: 0x00007ffff78281b0 libc.so.6`__libc_start_call_main + 130
    frame #12: 0x00007ffff7828279 libc.so.6`__libc_start_main@@GLIBC_2.34 + 139
    frame #13: 0x00000000004067a5 swc`_start at start.S:115
-------------------------------------------------------------
clang-format on */
void Manager::unify(Type::Ptr left, Type::Ptr right) noexcept {
  Variable *leftVar;
  Variable *rightVar;

  left = resolve(left, leftVar);
  right = resolve(right, rightVar);

  if (leftVar) {
    bind(leftVar->Name, right);
  } else if (rightVar) {
    bind(rightVar->Name, left);
  } else if (auto *leftArrow = dynamic_cast<Arrow *>(left.get()),
             *rightArrow = dynamic_cast<Arrow *>(right.get());
             leftArrow && rightArrow) {
    unify(leftArrow->Left, rightArrow->Left);
    unify(leftArrow->Right, rightArrow->Right);
  } else if (auto *leftID = dynamic_cast<Base *>(left.get()),
             *rightID = dynamic_cast<Base *>(right.get());
             leftID && rightID) {
    if (leftID->Name == rightID->Name)
      return;
  } else {
    panic("type checking failed: unification failed");
  }
}

void Manager::bind(const std::string &name, const Type::Ptr &type) noexcept {
  if (auto *other = dynamic_cast<const Variable *>(type.get());
      other && other->Name == name)
    return;
  Types[name] = type;
}

} // namespace swallow::type