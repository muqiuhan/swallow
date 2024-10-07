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

#include "context.h"
#include <llvm/ADT/APInt.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <format>
#include <memory>
#include <utility>

namespace swallow::compiler::ir::context
{
  void LLVMContext::CreateFunctions() noexcept
  {
    auto *voidType = llvm::Type::getVoidTy(Context);
    auto *sizeType = llvm::IntegerType::get(Context, sizeof(size_t) * 8);

    Functions.insert(
      {"Stack::Initialize",
       llvm::Function::Create(
         llvm::FunctionType::get(voidType, {StackPtrType}, false),
         llvm::Function::LinkageTypes::ExternalLinkage,
         "Stack::Initialize",
         &Module)});

    Functions.insert(
      {"Stack::Free",
       llvm::Function::Create(
         llvm::FunctionType::get(voidType, {StackPtrType}, false),
         llvm::Function::LinkageTypes::ExternalLinkage,
         "Stack::Free",
         &Module)});
  }

  void LLVMContext::CreateTypes() noexcept
  {
    StackType = llvm::StructType::create(Context, "Struct");
    StackPtrType = llvm::PointerType::getUnqual(StackType);
    TagType = llvm::IntegerType::getInt8Ty(Context);
  }

  inline static auto CreateNewFunction(
    const std::string &name,
    llvm::LLVMContext &Context,
    llvm::PointerType *StackPtrType,
    llvm::Module      &Module) noexcept -> std::pair<std::string, llvm::Function *>
  {
    auto *voidType = llvm::Type::getVoidTy(Context);
    auto *functionType = llvm::FunctionType::get(voidType, {StackPtrType}, false);
    auto  functionName = std::format("swallow_{}", name);
    auto  functionLinkageType = llvm::Function::LinkageTypes::ExternalLinkage;
    auto *function = llvm::Function::Create(functionType, functionLinkageType, functionName, &Module);

    return std::make_pair(functionName, function);
  }

  auto LLVMContext::CreateCustomFunction(const std::string &name, int32_t arity) noexcept -> llvm::Function *
  {
    auto &&[newFunctionName, newFunction] = CreateNewFunction(name, Context, StackPtrType, Module);
    auto *startBlock = llvm::BasicBlock::Create(Context, "EntryPoint", newFunction);
    auto  newCustomFunction = std::make_unique<CustomFunction>();

    newCustomFunction->Arity = arity;
    newCustomFunction->Function = newFunction;
    CustomFunctions.insert({newFunctionName, std::move(newCustomFunction)});

    return newFunction;
  }

  auto LLVMContext::CreateInt8(int8_t num) noexcept -> llvm::ConstantInt *
  {
    return llvm::ConstantInt::get(Context, llvm::APInt(8, 1));
  }

  auto LLVMContext::CreateInt32(int32_t num) noexcept -> llvm::ConstantInt * {}

  auto LLVMContext::CreateSize(size_t num) noexcept -> llvm::ConstantInt * {}

  auto LLVMContext::CreatePop(llvm::Function *function) noexcept -> llvm::Value *
  {
    return Builder.CreateCall(Functions.at("swallow::compiler::runtime::stack::Stack::Pop"), {function->arg_begin()});
  }

  auto LLVMContext::CreatePeek(llvm::Function *function, llvm::Value *value) noexcept -> llvm::Function * {}

  void LLVMContext::CreatePush(llvm::Function *function, llvm::Value *value) noexcept {}

  void LLVMContext::CreatePopN(llvm::Function *function, llvm::Value *value) noexcept {}

  void LLVMContext::CreateUpdate(llvm::Function *function, llvm::Value *value) noexcept {}

  void LLVMContext::CreatePack(llvm::Function *function, llvm::Value *value) noexcept {}

  void LLVMContext::CreateSplit(llvm::Function *function, llvm::Value *value) noexcept {}

  void LLVMContext::CreateSlide(llvm::Function *function, llvm::Value *value) noexcept {}

  void LLVMContext::CreateAlloc(llvm::Function *function, llvm::Value *value) noexcept {}

  auto LLVMContext::CreateEval(llvm::Value *value) noexcept -> llvm::Value * {}

  auto LLVMContext::CreateNum(llvm::Value *value) noexcept -> llvm::Value * {}

  auto LLVMContext::CreateGlobal(llvm::Value *value) noexcept -> llvm::Value * {}

  auto LLVMContext::CreateApp(llvm::Value *left, llvm::Value *right) noexcept -> llvm::Value * {}

  auto LLVMContext::UnwrapDataTag(llvm::Value *value) noexcept -> llvm::Value * {}

  auto LLVMContext::UnwrapNum(llvm::Value *value) noexcept -> llvm::Value *
  {
    auto *type = llvm::PointerType::getUnqual(StructTypes.at("swallow::compiler::runtime::node::Int"));
    auto *num = Builder.CreatePointerCast(value, type);

    return Builder.CreateLoad(type, Builder.CreateGEP(type, num, {CreateInt32(0), CreateInt32(1)}));
  }

} // namespace swallow::compiler::ir::context
