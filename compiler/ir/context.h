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

#ifndef SWALLOW_COMPILER_LLVM_H
#define SWALLOW_COMPILER_LLVM_H

#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <cstdint>
#include <map>
#include <memory>
#include <string>

namespace swallow::compiler::ir::context
{
  class LLVMContext
  {
  public:
    class CustomFunction
    {
    public:
      llvm::Function *Function;
      int32_t         Arity;
    };

  public:
    using Ptr = std::unique_ptr<CustomFunction>;

  public:
    llvm::LLVMContext Context;
    llvm::Module      Module;
    llvm::IRBuilder<> Builder;

  public:
    std::map<std::string, Ptr>                CustomFunctions;
    std::map<std::string, llvm::Function *>   Functions;
    std::map<std::string, llvm::StructType *> StructTypes;

  public:
    llvm::StructType   *StackType;
    llvm::PointerType  *StackPtrType;
    llvm::PointerType  *NodePtrType;
    llvm::IntegerType  *TagType;
    llvm::FunctionType *FunctionType;

  public:
    LLVMContext() : Builder(Context), Module("swallow", Context)
    {
      CreateTypes();
      CreateFunctions();
    }

  public:
    void CreateTypes() noexcept;

    void CreateFunctions() noexcept;

    auto CreateInt8(int8_t num) noexcept -> llvm::ConstantInt *;

    auto CreateInt32(int32_t num) noexcept -> llvm::ConstantInt *;

    auto CreateSize(size_t num) noexcept -> llvm::ConstantInt *;

    auto CreatePop(llvm::Function *function) noexcept -> llvm::Value *;

    auto CreatePeek(llvm::Function *function, llvm::Value *value) noexcept
      -> llvm::Function *;

    void CreatePush(llvm::Function *function, llvm::Value *value) noexcept;

    void CreatePopN(llvm::Function *function, llvm::Value *value) noexcept;

    void CreateUpdate(llvm::Function *function, llvm::Value *value) noexcept;

    void CreatePack(llvm::Function *function, llvm::Value *value) noexcept;

    void CreateSplit(llvm::Function *function, llvm::Value *value) noexcept;

    void CreateSlide(llvm::Function *function, llvm::Value *value) noexcept;

    void CreateAlloc(llvm::Function *function, llvm::Value *value) noexcept;

    auto CreateEval(llvm::Value *value) noexcept -> llvm::Value *;

    auto CreateNum(llvm::Value *value) noexcept -> llvm::Value *;

    auto CreateGlobal(llvm::Value *value) noexcept -> llvm::Value *;

    auto CreateApp(llvm::Value *left, llvm::Value *right) noexcept
      -> llvm::Value *;

    auto CreateCustomFunction(const std::string &name, int32_t arity) noexcept
      -> llvm::Function *;

    auto UnwrapDataTag(llvm::Value *value) noexcept -> llvm::Value *;

    auto UnwrapNum(llvm::Value *value) noexcept -> llvm::Value *;
  };
} // namespace swallow::compiler::ir::context

#endif
