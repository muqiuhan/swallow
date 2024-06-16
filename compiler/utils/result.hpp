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

#ifndef SWALLOW_COMPILER_UTILS_RESULT_RESULT_HPP
#define SWALLOW_COMPILER_UTILS_RESULT_RESULT_HPP

#include <iostream>
#include <optional>
#include <variant>

namespace swallow::compiler::utils
{
  template <typename T> struct Ok;
  template <typename E> struct Err;

  struct Void
  {};

  // Result is a type that represents either success (Ok) or failure (Err).
  template <typename T, typename E> struct Result
  {

    std::variant<Ok<T>, Err<E>> value;

    Result() = default;

    Result(const Ok<T> &val) : value(val) {}

    Result(const Err<E> &val) : value(val) {}

    friend auto operator<<(std::ostream &os, const Result &r) -> std::ostream &
    {
      if (r.is_ok())
        os << r.unwrap();
      else
        os << r.unwrap_err();
      return os;
    }

    auto operator=(Ok<T> val) -> Result
    {
      value = val;
      return *this;
    }

    auto operator=(Err<E> val) -> Result
    {
      value = val;
      return *this;
    }

    auto operator==(const Ok<T> &val) const -> bool { return is_ok() && unwrap() == val.value; }

    auto operator==(const Err<E> &val) const -> bool { return is_err() && unwrap_err() == val.value; }

    auto operator==(const Result<T, E> &rhs) const -> bool
    {
      return (
        (is_ok() && rhs.is_ok() && unwrap() == rhs.unwrap())
        || (is_err() && rhs.is_err() && unwrap_err() == rhs.unwrap_err()));
    }

    auto operator!=(const Result<T, E> &rhs) const -> bool { return !(*this == rhs); }

    // Returns true if the result is Ok.
    [[nodiscard]] auto is_ok() const -> bool { return std::holds_alternative<Ok<T>>(value); }

    // Returns true if the result is Err.
    [[nodiscard]] auto is_err() const -> bool { return std::holds_alternative<Err<E>>(value); }

    // Converts from Result<T, E> to std::optional<T>.
    [[nodiscard]] auto ok() const -> std::optional<T>
    {
      if (is_ok())
        return std::get<0>(value).value;
      return {};
    }

    // Converts from Result<T, E> to std::optional<E>.
    [[nodiscard]] auto err() const -> std::optional<E>
    {
      if (is_err())
        return std::get<1>(value).value;
      return {};
    }

    // Returns res if the result is Ok, otherwise returns the Err value of self.
    template <typename U> auto and_(const Result<U, E> &res) -> Result<U, E>
    {
      if (is_ok())
        return res;
      return Err(err().value());
    }

    // Synonymous with Result.and_(res)
    template <typename U> auto operator&&(const Result<U, E> &res) -> Result<U, E>
    {
      if (is_ok())
        return res;
      return Err(err().value());
    }

    // Calls op if the result is Ok, otherwise returns the Err value of self.
    // This function can be used for control flow based on Result values.
    template <typename Function> auto and_then(Function op) -> Result
    {
      if (is_ok())
        return op(unwrap());
      return Err(err().value());
    }

    // Returns res if the result is Err,
    // otherwise returns the Ok value of self.
    auto or_(const Result &res) -> Result
    {
      if (is_err())
        return res;
      return Ok(ok().value());
    }

    // Synonymous with Result.or_(res)
    auto operator||(const Result &res) -> Result
    {
      if (is_err())
        return res;
      return Ok(ok().value());
    }

    // Calls op if the result is Err,
    // otherwise returns the Ok value of self.
    template <typename Function> auto or_else(Function op) -> Result
    {
      if (is_err())
        return op(unwrap_err());
      return Ok(ok().value());
    }

    // Unwraps a result, yielding the content of an Ok. Else, it returns optb.
    auto unwrap_or(T optb) -> T
    {
      if (is_ok())
        return unwrap();
      return optb;
    }

    // Unwraps a result, yielding the content of an Ok.
    // If the value is an Err then it calls op with its value.
    template <typename Function> auto unwrap_or_else(Function op) -> T
    {
      if (is_ok())
        return unwrap();
      return op(unwrap_err());
    }

    auto contains(const T &this_value) -> bool { return is_ok() ? unwrap() == this_value : false; }

    auto contains_err(const E &this_value) -> bool { return is_err() ? unwrap_err() == this_value : false; }

    // Maps a Result<T, E> to Result<U, E> by
    // applying a function to a contained Ok value,
    // leaving an Err value untouched.
    //
    // This function can be used to compose the results of two functions.
    template <typename Function> auto map(Function fn) -> Result<decltype(fn(T())), E>
    {
      if (is_ok())
        return Result<decltype(fn(T())), E>(Ok<decltype(fn(T()))>(fn(unwrap())));
      return Result<decltype(fn(T())), E>(Err<E>(unwrap_err()));
    }

    // Applies a function to the contained value (if any),
    // or returns the provided default (if not).
    template <typename Value, typename Function> auto map_or(Value default_value, Function fn) -> decltype(fn(T()))
    {
      if (is_ok())
        return fn(unwrap());
      return default_value;
    }

    // Maps a Result<T, E> to U by applying a function to a contained Ok value,
    // or a fallback function to a contained Err value.
    //
    // This function can be used to unpack a successful result while handling an
    // error.
    template <typename ErrorFunction, typename OkFunction>
    auto map_or_else(ErrorFunction err_fn, OkFunction ok_fn) -> decltype(ok_fn(T()))
    {
      if (is_ok())
        return ok_fn(unwrap());
      return err_fn(unwrap_err());
    }

    // Maps a Result<T, E> to Result<T, F> by
    // applying a function to a contained Err value,
    // leaving an Ok value untouched.
    //
    // This function can be used to pass
    // through a successful result while handling an error.
    template <typename Function> auto map_err(Function fn) -> Result<T, decltype(fn(E()))>
    {
      if (is_err())
        return Result<T, decltype(fn(E()))>(Err<decltype(fn(E()))>(fn(unwrap_err())));
      return Result<T, decltype(fn(E()))>(Ok<T>(unwrap()));
    }

    // Unwraps a result, yielding the content of an Ok.
    // Throws if the value is an Err
    [[nodiscard]] auto unwrap() const
    {
      if (is_ok())
        return ok().value();
      throw err().value();
    }

    // Unwraps a result, ignore the content of an Ok.
    // Throws if the value is an Err
    [[nodiscard]] auto ignore() const
    {
      if (is_ok())
        {
          ok().value();
          return;
        }

      throw err().value();
    }

    // Unwraps a result, yielding the content of an Ok.
    // Throws if the value is an Err with the `msg` argument
    auto expect(const std::string &msg) -> T
    {
      if (is_ok())
        return unwrap();
      if constexpr (std::is_same_v<E, std::string>)
        throw msg + ": " + unwrap_err();
      else
        throw msg + ": " + std::to_string(unwrap_err());
    }

    // Unwraps a result, yielding the content of an Err.
    // Throws if the value is an Ok,
    [[nodiscard]] auto unwrap_err() const
    {
      if (is_err())
        return err().value();
      throw ok().value();
    }

    // Unwraps a result, yielding the content of an Err.
    // Throws if the value is an Ok with the `msg` argument
    auto expect_err(const std::string &msg) -> E
    {
      if (is_err())
        return unwrap_err();
      if constexpr (std::is_same_v<T, std::string>)
        throw msg + ": " + unwrap();
      else
        throw msg + ": " + std::to_string(unwrap());
    }

    // Returns the contained value or a default
    // if Ok, returns the contained value,
    // otherwise if Err, returns the default value for that type.
    [[nodiscard]] auto unwrap_or_default() const -> T
    {
      if (is_ok())
        return unwrap();
      return T();
    }
  };

  template <typename T> struct Ok
  {
    T value;

    Ok(T value) : value(value) {}

    template <typename Function> auto and_then(Function op) -> Result<T, T> { return Result<T, T>(*this).and_then(op); }
  };

  template <typename E> struct Err
  {
    E value;

    Err(E value) : value(value) {}

    template <typename Function> auto and_then(Function op) -> Result<E, E> { return Result<E, E>(*this).and_then(op); }
  };
} // namespace swallow::compiler::utils

#endif // SWALLOW_COMPILER_UTILS_RESULT_RESULT_HPP